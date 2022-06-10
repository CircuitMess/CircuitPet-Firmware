#include <Arduino.h>
#include <Chatter.h>
#include <CircuitOS.h>
#include <SPIFFS.h>
#include "src/Home/Sprites/BgSprite.h"
#include "src/Home/Sprites/StatsSprite.h"
#include "src/Home/Sprites/OSSprite.h"
Display* display;
Sprite* baseSprite;

void setup(){
	Serial.begin(115200);
	Chatter.begin();
	Input* input = Chatter.getInput();

	display = Chatter.getDisplay();
	baseSprite = display->getBaseSprite();

	baseSprite->clear(TFT_WHITE);

	//TESTING
	//background
	BgSprite bg(baseSprite,1);
	bg.push();

	//stats
	StatsSprite statsSprite(baseSprite, 50, 69, 83);
	statsSprite.push();

	//os
	OSSprite osSprite(baseSprite,1);
	osSprite.setPos(110, 5);
	osSprite.push();

	display->commit();

}

void loop(){
	LoopManager::loop();
}


