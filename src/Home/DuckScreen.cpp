#include "DuckScreen.h"
#include <Loop/LoopManager.h>
#include "../Stats/StatsManager.h"

static std::vector<MenuItem> menuItems;

DuckScreen::DuckScreen(Sprite* base) : State(), base(base), bgSprite(base, StatMan.getLevel()),
									   osSprite(base, StatMan.getLevel()),
									   characterSprite(base, StatMan.getLevel(), StatMan.get().oilLevel, Anim::General),
									   statsSprite(base, StatMan.get().happiness, StatMan.get().oilLevel, 100), //TODO - set battery level instead of '100' placeholder
									   menu(base, menuItems),
									   hider(&menu){

	osSprite.setPos(osX, osY);
	statsSprite.setPos(statsX, statsY);
	characterSprite.setPos(characterX, characterY);

}


void DuckScreen::onStart(){
	LoopManager::addListener(this); //Note - possible crash if start() is called before constructor finishes
}

void DuckScreen::onStop(){
	LoopManager::removeListener(this);
}

void DuckScreen::loop(uint micros){
	bgSprite.push();

	statsSprite.push();

	osSprite.push();

	characterSprite.loop(micros);
	characterSprite.push();

	base->push();
}

