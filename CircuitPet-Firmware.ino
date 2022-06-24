#include <Arduino.h>
#include <Chatter.h>
#include <CircuitOS.h>
#include <SPIFFS.h>
#include "src/Home/DuckScreen.h"

Display* display;
Sprite* baseSprite;

void setup(){
	Serial.begin(115200);
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


