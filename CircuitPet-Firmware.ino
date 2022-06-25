#include <Arduino.h>
#include <Chatter.h>
#include <CircuitOS.h>
#include <SPIFFS.h>
#include "src/Home/DuckScreen.h"

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

	Chatter.begin();
	Input* input = Chatter.getInput();

	display = Chatter.getDisplay();
	baseSprite = display->getBaseSprite();

	baseSprite->clear(TFT_BLACK);
	display->commit();

	baseSprite->setTextFont(0);
	baseSprite->setTextSize(0);

	auto duck = new DuckScreen(baseSprite);
	duck->start();
}

uint32_t t = 0;
void loop(){
	LoopManager::loop();

	uint32_t t2 = micros();
	float frameTime = (float) (t2 - t) / 1000000.0f;
	baseSprite->setCursor(1, 119);
	baseSprite->printf("%.1fms - %.1ffps\n", frameTime * 1000.0f, 1.0f / frameTime);
	t = t2;

	display->commit();
}


