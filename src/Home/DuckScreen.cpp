#include "DuckScreen.h"
#include <Loop/LoopManager.h>
#include "../Stats/StatsManager.h"
#include "../Games/TestGame.h"
#include "../Games/Game6/Game6.h"
#include "../Games/Game5.h"
#include <CircuitPet.h>

DuckScreen::DuckScreen(Sprite* base) : State(), base(base), characterSprite(base, StatMan.getLevel(), StatMan.get().oilLevel, Anim::General),
									   menu(base), hider(&menu){


	characterSprite.setPos(characterX, characterY);
}


void DuckScreen::onStart(){
	Input::getInstance()->addListener(this);
	LoopManager::addListener(this); //Note - possible crash if start() is called before constructor finishes
	hider.activity();

	//load resources
	bgSprite = std::make_unique<BgSprite>(base, StatMan.getLevel());
	osSprite = std::make_unique<OSSprite>(base, StatMan.getLevel());
	statsSprite = std::make_unique<StatsSprite>(base, StatMan.get().oilLevel,  StatMan.get().happiness, 100);
	osSprite->setPos(osX, osY);
	statsSprite->setPos(statsX, statsY);

	auto pushGame = [this](Game* game){
		stop();

		game->load();

		printf("Loading.");
		while(!game->isLoaded()){
			delay(500);
			printf(".");
		}
		printf("\n");
		printf("Free heap: %d B\n", ESP.getFreeHeap());

		printf("\nStarting...\n");

		LoopManager::loop();
		game->push(this);
	};

	menuItems = {
			{ "Oily", GameImage(base, "/MenuIcons/Icon1.raw"), [pushGame](){pushGame(new TestGame());}},
			{ "Flappy", GameImage(base, "/MenuIcons/Icon2.raw"), {} },
			{ "Eaty", GameImage(base, "/MenuIcons/Icon3.raw"), {} },
			{ "Jump & Duck", GameImage(base, "/MenuIcons/Icon4.raw"), {} },
			{ "Disco danceoff", GameImage(base, "/MenuIcons/Icon5.raw"), [pushGame](){pushGame(new Game5());} },
			{ "Space duck", GameImage(base, "/MenuIcons/Icon6.raw"), [pushGame](){pushGame(new Game6());}},
	};

	menu.setOffsetY(menuY);
	menu.setItems(menuItems);
}

void DuckScreen::onStop(){
	Input::getInstance()->removeListener(this);
	LoopManager::removeListener(this);

	//release resources
	bgSprite.reset();
	osSprite.reset();
	statsSprite.reset();
	menuItems.clear();
}

void DuckScreen::loop(uint micros){
	bgSprite->push();

	statsSprite->push();

	osSprite->push();

	characterSprite.loop(micros);
	characterSprite.push();

	menu.push();
}

void DuckScreen::buttonPressed(uint i){
	hider.activity();

	switch(i){
		case BTN_LEFT:
			selection = menu.prev();
			break;
		case BTN_RIGHT:
			selection = menu.next();
			break;
		case BTN_A: {
			if(hider.getState() != MenuHider::Shown) return;

			auto func = menuItems[selection].primary;
			if(func) func();
			return;
		}
	}
}

