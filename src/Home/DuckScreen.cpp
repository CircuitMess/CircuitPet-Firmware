#include "DuckScreen.h"
#include <Loop/LoopManager.h>
#include "../Stats/StatsManager.h"
#include "../Games/Game1/Game1.h"
#include "../Games/Game3/Game3.h"
#include "../Games/Game6/Game6.h"
#include "../Games/Game5.h"
#include "../Games/2/Game2.h"
#include "../DeathState.h"
#include <CircuitPet.h>

DuckScreen::DuckScreen(Sprite* base) : State(), base(base), characterSprite(base, StatMan.getLevel(), StatMan.get().oilLevel < rustThreshold, Anim::General),
									   menu(base), hider(&menu){


	characterSprite.setPos(characterX, characterY);
}

void DuckScreen::onStart(){
	Input::getInstance()->addListener(this);

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
			{ "Oily",1, GameImage(base, "/MenuIcons/Icon1.raw"), GameImage(base, "/MenuIcons/Icon1.raw"), [pushGame](){pushGame(new Game1());}},
			{ "Flappy",2, GameImage(base, "/MenuIcons/Icon2.raw"),GameImage(base, "/MenuIcons/Locked2.raw"),  [pushGame](){pushGame(new Game2());} },
			{ "Eaty", 3,GameImage(base, "/MenuIcons/Icon3.raw"),GameImage(base, "/MenuIcons/Locked3.raw"),  [pushGame](){ pushGame(new Game3()); } },
			{ "Jump & Duck",4, GameImage(base, "/MenuIcons/Icon4.raw"), GameImage(base, "/MenuIcons/Locked4.raw"), {} },
			{ "Disco danceoff", 5,GameImage(base, "/MenuIcons/Icon5.raw"), GameImage(base, "/MenuIcons/Locked5.raw"), [pushGame](){pushGame(new Game5());} },
			{ "Space duck", 6,GameImage(base, "/MenuIcons/Icon6.raw"), GameImage(base, "/MenuIcons/Locked6.raw"), [pushGame](){pushGame(new Game6());}},
	};

	menu.setOffsetY(menuY);
	menu.setItems(menuItems);


	LoopManager::loop();
	LoopManager::addListener(this); //Note - possible crash if start() is called before constructor finishes
	hider.activity();

	currentStats = targetStats = prevStats = StatMan.get();
	StatMan.addListener(this);
	StatMan.setPaused(false);

	characterSprite.setRusty(StatMan.get().oilLevel < rustThreshold);
	characterSprite.setCharLevel(StatMan.getLevel());
	characterSprite.setAnim(Anim::General);

	randInterval = rand() % 4000000 + 2000000;

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

	if(dead){
		volatile auto temp = base;
		stop();
		delete this;

		auto duck = new DeathState(temp);
		duck->start();
		return;
	}

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

		statsSprite->setHappiness(currentStats.happiness);
		statsSprite->setOilLevel(currentStats.oilLevel);
	}

	//playing random duck animations while idling
	randCounter += micros;
	if(randCounter >= randInterval){
		randCounter = 0;
		Anim anim;
		if(!specialAnimPlaying){
			specialAnimPlaying = true;
			randInterval = 1000000;
			int num = 1 + rand() % ((uint8_t)Anim::Count - 1);
			anim = (Anim)(num);
		}else{
			specialAnimPlaying = false;
			randInterval = rand() % 4000000 + 2000000;
			anim = Anim::General;
		}

		characterSprite.setAnim(anim);
	}

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
			if(menu.isShaking()) return;
			selection = menu.prev();
			break;
		case BTN_RIGHT:
			if(menu.isShaking()) return;
			selection = menu.next();
			break;
		case BTN_A:{
			if(hider.getState() != MenuHider::Shown) return;
			if(menuItems[selection].levelRequired > StatMan.getLevel()){
				menu.shake();
				return;
			}
			auto func = menuItems[selection].primary;
			if(func) func();
			return;
		}
	}
}

void DuckScreen::statsChanged(const Stats& stats, bool leveledUp){

	if(StatMan.hasDied()){
		dead = true;
		StatMan.setPaused(true);
		return;
	}

	if(leveledUp){
		//TODO - levelup anims
		bgSprite->setLevel(StatMan.getLevel());
		characterSprite.setCharLevel(StatMan.getLevel());
		osSprite->setLevel(StatMan.getLevel());
	}

	characterSprite.setRusty(stats.oilLevel < rustThreshold);

	targetStats = stats;
	prevStats = currentStats;

	easeTimer = 0;
}
