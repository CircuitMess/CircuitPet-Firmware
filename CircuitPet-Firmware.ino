#include <Arduino.h>
#include <Chatter.h>
#include <CircuitOS.h>
#include <SPIFFS.h>
#include "src/Home/Menu/Menu.h"
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
	/*
	char imgPath[50];
	sprintf(imgPath, "/MenuIcons/prilika.raw");
	fs::File itemFile = SPIFFS.open(imgPath);

	Sprite sprite(baseSprite, 32, 32);

	sprite.clear(TFT_TRANSPARENT);
	sprite.drawIcon(itemFile,0, 0, 32, 32);
	sprite.push();
	*/

	std::vector<MenuItem> items;
	GameImage gi(baseSprite,"/MenuIcons/prilika.raw");
	MenuItem item("Name", gi);

	GameImage gi2(baseSprite,"/MenuIcons/nesto.raw");
	MenuItem item2("Fame", gi2);

	GameImage gi3(baseSprite,"/MenuIcons/nesto.raw");
	MenuItem item3("Fame", gi3);

	GameImage gi4(baseSprite,"/MenuIcons/nesto.raw");
	MenuItem item4("Fame", gi4);

	items.push_back(item);
	items.push_back(item2);
	items.push_back(item3);
	items.push_back(item4);
	items.push_back(item4);

	Menu menu(baseSprite,items);
	menu.draw();

	display->commit();

}

void loop(){
LoopManager::loop();
}


