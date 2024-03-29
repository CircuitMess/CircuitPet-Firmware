#include <SPIFFS.h>
#include <Settings.h>
#include "JigHWTest.h"
#include "SPIFFSChecksum.hpp"
#include <Pins.hpp>
#include <CircuitPet.h>
#include <soc/efuse_reg.h>
#include <esp_efuse.h>
#include <Wire.h>
#include <Battery/BatteryService.h>
#include <ctime>
#include <Notes.h>

JigHWTest* JigHWTest::test = nullptr;

JigHWTest::JigHWTest(Display* display) : display(display), canvas(display->getBaseSprite()){
	test = this;

	tests.push_back({ JigHWTest::RTCTest, "RTC", [](){}});
	tests.push_back({ JigHWTest::Time, "Time", [](){}});
	tests.push_back({JigHWTest::SPIFFSTest, "SPIFFS", [](){ }});
	tests.push_back({ JigHWTest::BatteryCalib, "Bat calib", [](){}});
	tests.push_back({JigHWTest::BatteryCheck, "Bat check", [](){}});
}

void JigHWTest::start(){
	Serial.println();
	Serial.printf("TEST:begin:%llx\n", ESP.getEfuseMac());

	canvas->clear(TFT_BLACK);
	canvas->setTextColor(TFT_GOLD);
	canvas->setTextWrap(false, false);
	canvas->setTextDatum(textdatum_t::middle_center);

	canvas->setTextFont(0);
	canvas->setTextSize(1);
	canvas->setCursor(0, 6);

	canvas->printCenter("CircuitPet Hardware Test");
	canvas->setCursor(canvas->width() / 2, 16);
	canvas->println();
	display->commit();

	bool pass = true;
	for(const Test& test : tests){
		currentTest = test.name;

		canvas->setTextColor(TFT_WHITE);
		canvas->printf("%s: ", test.name);
		display->commit();

		Serial.printf("TEST:startTest:%s\n", currentTest);

		bool result = test.test();

		canvas->setTextColor(result ? TFT_GREEN : TFT_RED);
		canvas->printf("%s\n", result ? "PASSED" : "FAILED");
		display->commit();

		Serial.printf("TEST:endTest:%s\n", result ? "pass" : "fail");

		if(!(pass &= result)){
			if(test.onFail){
				test.onFail();
			}

			break;
		}
	}

	if(!pass){
		Serial.printf("TEST:fail:%s\n", currentTest);
		for(;;);
	}

	Serial.println("TEST:passall");

	canvas->print("\n\n");
	canvas->setTextColor(TFT_GREEN);
	canvas->printCenter("All OK!");
	display->commit();


	Sound s = {{ 350, 400, 200 },
			   { 800, 800, 500 },
			   { 1000, 1000, 500 },
			   { 1200, 1200, 500 }
	};
//	Audio.play(s);
}

void JigHWTest::log(const char* property, const char* value){
	Serial.printf("%s:%s:%s\n", currentTest, property, value);
}

void JigHWTest::log(const char* property, float value){
	Serial.printf("%s:%s:%f\n", currentTest, property, value);
}

void JigHWTest::log(const char* property, double value){
	Serial.printf("%s:%s:%lf\n", currentTest, property, value);
}

void JigHWTest::log(const char* property, bool value){
	Serial.printf("%s:%s:%s\n", currentTest, property, value ? "TRUE" : "FALSE");
}

void JigHWTest::log(const char* property, uint32_t value){
	Serial.printf("%s:%s:%lu\n", currentTest, property, value);
}

void JigHWTest::log(const char* property, int32_t value){
	Serial.printf("%s:%s:%ld\n", currentTest, property, value);
}

void JigHWTest::log(const char* property, const String& value){
	Serial.printf("%s:%s:%s\n", currentTest, property, value.c_str());
}

bool JigHWTest::RTCTest(){
	Wire.beginTransmission(I2C_BM8563_DEFAULT_ADDRESS);
	uint8_t error = Wire.endTransmission();

	if(error == 0){
		test->log("begin", true);
	}else{
		test->log("begin", false);
		return false;
	}

	auto t = CircuitPet.getUnixTime();
	delay(2000);
	auto diff = std::difftime(CircuitPet.getUnixTime(), t);

	if(diff == 1 || diff == 2){
		test->log("time passage", true);
	}else{
		test->log("time passage (expected 1s or 2s)", (uint32_t)diff);
		return false;
	}

	return true;
}

bool JigHWTest::Time(){
	static constexpr size_t count = 1000;

	I2C_BM8563_DateTypeDef dateStruct;
	I2C_BM8563_TimeTypeDef timeStruct;
	time_t lastTime;

	auto rtc = CircuitPet.getRTC();
	for(int i = 0; i < count; i++){
		rtc.getDate(&dateStruct);
		rtc.getTime(&timeStruct);

		std::tm t { timeStruct.seconds,
					timeStruct.minutes,
					timeStruct.hours,
					dateStruct.date,
					dateStruct.month - 1,
					dateStruct.year - 1900};

		auto time = mktime(&t);

		if(i == 0){
			lastTime = time;
			continue;
		}

		auto diff = abs(difftime(time, lastTime));
		if(diff > 10.0f){
			test->log("reading", i);
			test->log("diff", diff);
			return false;
		}

		lastTime = time;
	}

	return true;
}

bool JigHWTest::BatteryCalib(){
	if(Battery.getVoltOffset() != 0){
		test->log("calibrated", Battery.getVoltOffset());
		CircuitPet.getDisplay()->getBaseSprite()->print("fused. ");
		return true;
	}

	constexpr uint16_t numReadings = 50;
	constexpr uint16_t readDelay = 50;
	uint32_t reading = 0;

	for(int i = 0; i < numReadings; i++){
		reading += analogRead(PIN_BATT);
		delay(readDelay);
	}
	reading /= numReadings;


	uint32_t mapped = Battery.mapReading(reading);

	int16_t offset = referenceVoltage - mapped;

	test->log("reading", reading);
	test->log("mapped", mapped);
	test->log("offset", offset);

	if(abs(offset) >= 1000){
		test->log("offset too big, read voltage: ", (uint32_t)reading);
		return false;
	}

	uint16_t offsetLow = offset & 0b01111111;
	uint16_t offsetHigh = offset >> 7;

	REG_SET_FIELD(EFUSE_BLK3_WDATA3_REG, EFUSE_ADC1_TP_LOW, offsetLow);
	REG_SET_FIELD(EFUSE_BLK3_WDATA3_REG, EFUSE_ADC1_TP_HIGH, offsetHigh);
	esp_efuse_burn_new_values();
	esp_efuse_reset();

	return true;
}

bool JigHWTest::BatteryCheck(){
	constexpr uint16_t numReadings = 50;
	constexpr uint16_t readDelay = 10;
	uint32_t reading = 0;

	for(int i = 0; i < numReadings; i++){
		reading += analogRead(PIN_BATT);
		delay(readDelay);
	}
	reading /= numReadings;

	uint32_t voltage = Battery.mapReading(reading) + Battery.getVoltOffset();
	if(voltage < referenceVoltage - 100 || voltage > referenceVoltage + 100){
		test->log("raw", reading);
		test->log("mapped", Battery.mapReading(reading));
		test->log("offset", Battery.getVoltOffset());
		test->log("mapped+offset", voltage);
		return false;
	}

	return true;
}

bool JigHWTest::SPIFFSTest(){
	for(const auto& f : SPIFFSChecksums){
		if(!SPIFFS.exists(f.name)){
			test->log("missing", f.name);
			return false;
		}

		fs::File file = SPIFFS.open(f.name, "r");
		uint32_t sum = calcChecksum(file);
		file.close();

		if(sum != f.sum){
			test->log("file", f.name);
			test->log("expected", (uint32_t)f.sum);
			test->log("got", (uint32_t)sum);

			return false;
		}
	}

	return true;
}

uint32_t JigHWTest::calcChecksum(File& file){
	if(!file) return 0;

#define READ_SIZE 512

	uint32_t sum = 0;
	uint8_t b[READ_SIZE];
	size_t read = 0;
	while(read = file.read(b, READ_SIZE)){
		for(int i = 0; i < read; i++){
			sum += b[i];
		}
	}

	return sum;
}


