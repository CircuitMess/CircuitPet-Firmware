#include "DuckScreen.h"
#include <Loop/LoopManager.h>
#include "../Stats/StatsManager.h"
#include "../Games/TestGame.h"
#include <Chatter.h>

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

	currentStats = targetStats = prevStats = StatMan.get();
	StatMan.addListener(this);
}


void DuckScreen::onStart(){
	Input::getInstance()->addListener(this);
	LoopManager::addListener(this); //Note - possible crash if start() is called before constructor finishes
	hider.activity();
}

void DuckScreen::onStop(){
	Input::getInstance()->removeListener(this);
	LoopManager::removeListener(this);
}

void DuckScreen::loop(uint micros){

	//stats display easing when a change occurs
	if(currentStats != targetStats){

		easeTimer += micros / 1000000.0;
		float x = easeTimer / easeTime;

		if(x >= 1.f){
			currentStats = targetStats;
		}else{

			float ease = 1.0f - cos((x * PI) / 2);


			currentStats.oilLevel = prevStats.oilLevel + ((float)(targetStats.oilLevel - prevStats.oilLevel)) * ease;
			currentStats.happiness = prevStats.happiness + ((float)(targetStats.happiness - prevStats.happiness)) * ease;
		}

		statsSprite.setHappiness(currentStats.happiness);
		statsSprite.setOilLevel(currentStats.oilLevel);
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
			menu.prev();
			break;
		case BTN_RIGHT:
			menu.next();
			break;
		case BTN_A: {
			auto func = menuItems[menu.getSelectedIndex()].primary;
			if(func) func();
			return;
		}
	}
}

void DuckScreen::statsChanged(const Stats& stats, bool leveledUp){
	if(leveledUp){
		//TODO - levelup anims
	}

	targetStats = stats;
	prevStats = currentStats;

	easeTimer = 0;
}

