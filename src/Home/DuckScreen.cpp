#include "DuckScreen.h"
#include <Loop/LoopManager.h>
#include "../Stats/StatsManager.h"
#include <Chatter.h>

static std::vector<MenuItem> menuItems;

const char* imagePaths[] = { "/MenuIcons/prilika.raw", "/MenuIcons/nesto.raw", "/MenuIcons/nesto.raw", "/MenuIcons/slika.raw" };

DuckScreen::DuckScreen(Sprite* base) : State(), base(base), bgSprite(base, StatMan.getLevel()),
									   osSprite(base, StatMan.getLevel()),
									   characterSprite(base, StatMan.getLevel(), StatMan.get().oilLevel, Anim::General),
									   statsSprite(base, StatMan.get().happiness, StatMan.get().oilLevel, 100), //TODO - set battery level instead of '100' placeholder
									   menu(base), hider(&menu){

	osSprite.setPos(osX, osY);
	statsSprite.setPos(statsX, statsY);
	characterSprite.setPos(characterX, characterY);
	menu.setOffsetY(menuY);

	for(auto path : imagePaths){
		menuItems.emplace_back("Name", GameImage(base, path));
	}
	menu.setItems(menuItems);
}


void DuckScreen::onStart(){
	Input::getInstance()->addListener(this);
	LoopManager::addListener(this); //Note - possible crash if start() is called before constructor finishes
}

void DuckScreen::onStop(){
	Input::getInstance()->removeListener(this);
	LoopManager::removeListener(this);
}

void DuckScreen::loop(uint micros){
	bgSprite.push();

	statsSprite.push();

	osSprite.push();

	characterSprite.loop(micros);
	characterSprite.push();

	menu.draw();

	base->push();
}

void DuckScreen::buttonPressed(uint i){
	hider.activity();

	switch(i){
		case BTN_LEFT:
			menu.prev();
			break;
		case BTN_RIGHT:
			menu.next();
			break;
		case BTN_A:
			//start selected game
			break;
		default:
			break;
	}
}

