#include "DuckScreen.h"
#include <Loop/LoopManager.h>
#include "../Stats/StatsManager.h"
#include "../Games/Game1/Game1.h"
#include "../Games/Game3/Game3.h"
#include "../Games/Game6/Game6.h"
#include "../Games/Game5.h"
#include "../Games/2/Game2.h"
#include "../Games/Game4/Game4.h"
#include "../DeathState.h"
#include <CircuitPet.h>
#include <SPIFFS.h>
#include <FS/RamFile.h>
#include "../Settings/SettingsScreen.h"

DuckScreen::DuckScreen(Sprite* base) : State(), base(base),
									   menu(base), hider(&menu){


}

void DuckScreen::onStart(){
	Input::getInstance()->addListener(this);

	//load resources
	bgSprite = std::make_unique<BgSprite>(base, StatMan.getLevel());
	osSprite = std::make_unique<OSSprite>(base, StatMan.getLevel());
	statsSprite = std::make_unique<StatsSprite>(base, StatMan.get().oilLevel,  StatMan.get().happiness, 100);
	characterSprite = std::make_unique<CharacterSprite>(base, StatMan.getLevel(), StatMan.get().oilLevel < rustThreshold, Anim::General);

	osSprite->setPos(osX, osY);
	statsSprite->setPos(statsX, statsY);
	characterSprite->setPos(characterX, characterY);

	characterSprite->setRusty(StatMan.get().oilLevel < rustThreshold);
	characterSprite->setCharLevel(StatMan.getLevel());
	characterSprite->setAnim(Anim::General);

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

		LoopManager::resetTime();
		game->push(this);
	};

	menuItems = {
			{ "Oily",1, GameImage(base, "/MenuIcons/Icon1.raw"), GameImage(base, "/MenuIcons/Icon1.raw"), [pushGame](){pushGame(new Game1());}},
			{ "Flappy",2, GameImage(base, "/MenuIcons/Icon2.raw"),GameImage(base, "/MenuIcons/Locked2.raw"),  [pushGame](){pushGame(new Game2());} },
			{ "Eaty", 3,GameImage(base, "/MenuIcons/Icon3.raw"),GameImage(base, "/MenuIcons/Locked3.raw"),  [pushGame](){ pushGame(new Game3()); } },
			{ "Jump & Duck",4, GameImage(base, "/MenuIcons/Icon4.raw"), GameImage(base, "/MenuIcons/Locked4.raw"), [pushGame](){pushGame(new Game4::Game4());} },
			{ "Disco danceoff", 5,GameImage(base, "/MenuIcons/Icon5.raw"), GameImage(base, "/MenuIcons/Locked5.raw"), [pushGame](){pushGame(new Game5());} },
			{ "Space duck", 6,GameImage(base, "/MenuIcons/Icon6.raw"), GameImage(base, "/MenuIcons/Locked6.raw"), [pushGame](){pushGame(new Game6());} },
			{ "Settings", 1,  GameImage(base, "/MenuIcons/settings.raw"), GameImage(base, "/MenuIcons/settings.raw"), [this](){
				auto settings = new SettingsScreen::SettingsScreen(*CircuitPet.getDisplay());
				settings->push(this);
			}}
	};

	menu.setItems(menuItems);


	LoopManager::resetTime();
	LoopManager::addListener(this); //Note - possible crash if start() is called before constructor finishes
	hider.hide();
	hider.activity();

	currentStats = targetStats = prevStats = StatMan.get();
	StatMan.addListener(this);
	StatMan.setPaused(false);

	randInterval = rand() % 4000000 + 2000000;

}

void DuckScreen::onStop(){
	Input::getInstance()->removeListener(this);
	LoopManager::removeListener(this);
	StatMan.removeListener(this);

	//release resources
	bgSprite.reset();
	osSprite.reset();
	statsSprite.reset();
	characterSprite.reset();
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

	if(luState != None){
		hider.activity();

		if(luState == FadeIn || luState == FadeOut){
			if(luMicros == 0){
				luMicros = ::micros();
				return;
			}

			bgSprite->push();
			statsSprite->push();
			osSprite->push();
			characterSprite->push();
			menu.push();

			luFile.seek(0);

			float t = (float) (::micros() - luMicros) / 500000.0f;
			if(t >= 1.0f){
				if(luState == FadeIn){
					luState = Image;
					base->drawIcon(luFile, 0, 0, 160, 128);
					luMicros = ::micros();
				}else if(luState == FadeOut){
					luState = None;
					luMicros = 0;
					luFile.close();
				}

				return;
			}

			if(luState == FadeOut){
				t = 1.0f - t;
			}

			luFile.seek(0);
			for(int i = 0; i < 160 * 128; i++){
				int y = i / 160;
				int x = i - y * 160;

				Color target;
				luFile.read(reinterpret_cast<uint8_t*>(&target), 2);

				Color original = base->readPixel(x, y);

				uint8_t oR = (original >> 11) & 0b11111;
				uint8_t oG = (original >> 5) & 0b111111;
				uint8_t oB = original & 0b11111;

				uint8_t tR = (target >> 11) & 0b11111;
				uint8_t tG = (target >> 5) & 0b111111;
				uint8_t tB = target & 0b11111;

				uint8_t r = oR + (tR - oR) * t;
				uint8_t g = oG + (tG - oG) * t;
				uint8_t b = oB + (tB - oB) * t;

				uint16_t computed =
						(r & 0b11111) << 11 |
						(g & 0b111111) << 5 |
						(b & 0b11111);

				base->writePixel(x, y, computed);
			}
		}else if(luState == Image){
			if(!luApplied){
				currentStats = targetStats = prevStats = StatMan.get();

				characterSprite->setRusty(currentStats.oilLevel < rustThreshold);
				characterSprite->setCharLevel(StatMan.getLevel());
				bgSprite->setLevel(StatMan.getLevel());
				osSprite->setLevel(StatMan.getLevel());
				luApplied = true;

				menu.repos();
			}

			if(::micros() - luMicros >= 3000000){
				luState = FadeOut;
				luMicros = ::micros();
			}
		}

		return;
	}

	//stats display easing when a change occurs
	if(currentStats != targetStats){

		easeTimer += micros / 1000000.0;
		float x = easeTimer / easeTime;

		if(x >= 1.f){
			currentStats = targetStats;
		}else{
			float ease = 1.0f - cos((x * PI) / 2.0);

			currentStats.oilLevel = prevStats.oilLevel + ((float)((int)(targetStats.oilLevel) - (int)(prevStats.oilLevel))) * ease;
			currentStats.happiness = prevStats.happiness + ((float)((int)(targetStats.happiness) - (int)(prevStats.happiness))) * ease;
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

		characterSprite->setAnim(anim);
	}

	bgSprite->push();

	statsSprite->push();

	osSprite->push();

	characterSprite->loop(micros);
	characterSprite->push();

	menu.push();
}

void DuckScreen::buttonPressed(uint i){
	if(luState != None) return;

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
		File file = SPIFFS.open(String("/LevelUp/") + StatMan.getLevel() + ".raw");
		luFile = RamFile::open(file);

		luState = FadeIn;
		luMicros = 0;
		luApplied = false;
		return;
	}

	characterSprite->setRusty(stats.oilLevel < rustThreshold);

	targetStats = stats;
	prevStats = currentStats;

	easeTimer = 0;
}
