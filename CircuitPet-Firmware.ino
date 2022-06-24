#include <Arduino.h>
#include <Chatter.h>
#include <CircuitOS.h>
#include <SPIFFS.h>
#include "src/Home/Menu/Menu.h"
#include "src/Home/Menu/MenuHider.h"

Display* display;
Sprite* baseSprite;
Menu* menu;
MenuHider* hider;

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

	menu = new Menu(baseSprite,items);
	menu->setOffsetY(64);
	menu->push();


	input->setBtnPressCallback(BTN_L, [](){
		menu->prev();
		hider->activity();
	});

	input->setBtnPressCallback(BTN_R, [](){
		menu->next();
		hider->activity();
	});

	hider = new MenuHider(menu);

	display->commit();

}

void loop(){
	baseSprite->clear(TFT_WHITE);
	LoopManager::loop();
	menu->push();
	display->commit();
}


