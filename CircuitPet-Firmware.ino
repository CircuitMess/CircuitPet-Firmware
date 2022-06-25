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

	auto duck = new DuckScreen(baseSprite);
	duck->start();
}

void loop(){
	LoopManager::loop();
	display->commit();
}


