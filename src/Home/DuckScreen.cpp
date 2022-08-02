#include "DuckScreen.h"
#include <Loop/LoopManager.h>
#include "../Stats/StatsManager.h"
#include "../Games/TestGame.h"
#include <CircuitPet.h>

DuckScreen::DuckScreen(Sprite* base) : State(), base(base), bgSprite(base, StatMan.getLevel()),
									   osSprite(base, StatMan.getLevel()),
									   characterSprite(base, StatMan.getLevel(), StatMan.get().oilLevel, Anim::General),
									   statsSprite(base, StatMan.get().oilLevel,  StatMan.get().happiness, 100), //TODO - set battery level instead of '100' placeholder
									   menu(base), hider(&menu){

	osSprite.setPos(osX, osY);
	statsSprite.setPos(statsX, statsY);
	characterSprite.setPos(characterX, characterY);
	menu.setOffsetY(menuY);

	menuItems = {
			{ "Oily", GameImage(base, "/MenuIcons/Icon1.raw"), [this](){
				Game* game = new TestGame();
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
			} },
			{ "Flappy", GameImage(base, "/MenuIcons/Icon2.raw"), {} },
			{ "Eaty", GameImage(base, "/MenuIcons/Icon3.raw"), {} },
			{ "Jump & Duck", GameImage(base, "/MenuIcons/Icon4.raw"), {} },
			{ "Disco danceoff", GameImage(base, "/MenuIcons/Icon5.raw"), {} },
			{ "Space duck", GameImage(base, "/MenuIcons/Icon6.raw"), {} },
	};

	menu.setItems(menuItems);
}


void DuckScreen::onStart(){
	Input::getInstance()->addListener(this);
	LoopManager::addListener(this); //Note - possible crash if start() is called before constructor finishes
	hider.activity();

	characterSprite.setRusty(StatMan.get().oilLevel < 25);
	characterSprite.setCharLevel(StatMan.getLevel());
	characterSprite.setAnim(Anim::General);

	randInterval = rand() % 4000000 + 2000000;

}

void DuckScreen::onStop(){
	Input::getInstance()->removeListener(this);
	LoopManager::removeListener(this);
}

void DuckScreen::loop(uint micros){
	randCounter+=micros;
	if(randCounter >= randInterval){
		randCounter = 0;
		Anim anim;
		if(!specialAnimPlaying){
			specialAnimPlaying = true;
			randInterval = 1000000;
			int num = 1 + rand() % ((uint8_t)Anim::Count-1);
			anim = (Anim)(num);
		}else{
			specialAnimPlaying = false;
			randInterval = rand() % 4000000 + 2000000;
			anim = Anim::General;
		}

		characterSprite.setAnim(anim);
	}

	bgSprite.push();

	statsSprite.push();

	osSprite.push();

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

