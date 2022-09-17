#include <Arduino.h>
#include <CircuitPet.h>
#include <Loop/LoopManager.h>
#include <CircuitOS.h>
#include <SPIFFS.h>
#include "src/Intro.h"
#include "src/Stats/StatsManager.h"
#include "src/Clock/ClockMaster.h"
#include <Settings.h>
#include "src/UserHWTest.h"
#include "src/JigHWTest/JigHWTest.h"
#include "src/AutoShutdown.h"
#include <Audio/Piezo.h>

extern "C" {
#include <bootloader_random.h>
}


bool checkJig(){
	char buf[7];
	int wp = 0;

	uint32_t start = millis();
	while(millis() - start < 1000){
		while(Serial.available()){
			buf[wp] = Serial.read();
			wp = (wp + 1) % 7;

			for(int i = 0; i < 7; i++){
				int match = 0;
				static const char* target = "JIGTEST";

				for(int j = 0; j < 7; j++){
					match += buf[(i+j) % 7] == target[j];
				}

				if(match == 7) return true;
			}
		}
	}

	return false;
}

Display* display;
Sprite* baseSprite;

void initLog(){
	esp_log_level_set("*", ESP_LOG_NONE);

	const static auto tags = { "*" };

	for(const char* tag : tags){
		esp_log_level_set(tag, ESP_LOG_VERBOSE);
	}
}

void setup(){
	Serial.begin(115200);
	initLog();

	bootloader_random_enable();
	srand(esp_random());
	bootloader_random_disable();

	LoopManager::reserve(12);

	CircuitPet.begin(false);
	Input* input = CircuitPet.getInput();

	display = CircuitPet.getDisplay();
	baseSprite = display->getBaseSprite();

	if(checkJig()){
		CircuitPet.fadeIn(0);
		printf("Jig\n");
		auto test = new JigHWTest(display);
		test->start();
		for(;;);
	}


	baseSprite->clear(TFT_BLACK);
	display->commit();

	baseSprite->setTextFont(0);
	baseSprite->setTextSize(0);


	Clock.begin();
	StatMan.begin();

	Clock.updatePersistent();

	auto shutdown = new AutoShutdown();
	shutdown->begin();

	if(!Settings.get().hwTested){
		auto test = new UserHWTest(baseSprite, [](){
			Settings.get().hwTested = true;
			Settings.store();
			CircuitPet.fadeOut();

			auto intro = new Intro(baseSprite);
			LoopManager::resetTime();
			intro->start();
			display->commit();
			CircuitPet.fadeIn();
		});
		test->start();
		CircuitPet.fadeIn();
	}else{
		Piezo.setMute(!Settings.get().sound);
		auto intro = new Intro(baseSprite);
		LoopManager::loop();
		intro->start();
		CircuitPet.fadeIn();
	}
}

uint32_t t = 0;
void loop(){
	LoopManager::loop();

	uint32_t t2 = micros();
	float frameTime = (float) (t2 - t) / 1000000.0f;
	baseSprite->setTextFont(0);
	baseSprite->setTextSize(0);
	baseSprite->setTextColor(TFT_WHITE);
	baseSprite->setCursor(1, 119);
	auto s = StatMan.get();
	baseSprite->printf("%d;1:%d,2:%d,3:%d,4:%d \n", CircuitPet.reads, CircuitPet.mistakes[0],  CircuitPet.mistakes[1], CircuitPet.mistakes[2], CircuitPet.mistakes[3]);
	t = t2;

	display->commit();
}

