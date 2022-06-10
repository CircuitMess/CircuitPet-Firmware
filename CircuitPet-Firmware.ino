#include <Arduino.h>
#include <Chatter.h>
#include <CircuitOS.h>
#include <SPIFFS.h>
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

	display->commit();

}

void loop(){
	LoopManager::loop();
}


