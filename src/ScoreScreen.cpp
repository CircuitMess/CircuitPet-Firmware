#include "ScoreScreen.h"
#include "Stats/StatsManager.h"
#include "Home/Sprites/BgSprite.h"
#include <CircuitPet.h>
#include <SPIFFS.h>
#include <Input/Input.h>
#include <Loop/LoopManager.h>
#include <FS/RamFile.h>

ScoreScreen::ScoreScreen(Stats stats) : base(CircuitPet.getDisplay()->getBaseSprite()), stats(stats), oil(base, StatSprite::OilLevel, StatMan.get().oilLevel),
										happiness(base, StatSprite::Happiness, StatMan.get().happiness),
										xp(base, StatSprite::XPLevel, StatMan.getExpPercentage()){

	currentStats = StatMan.get();
	targetStats = stats + currentStats;
	prevStats = currentStats;

	frameFile = RamFile::open(SPIFFS.open("/frame.raw"));
}

void ScoreScreen::onStart(){
	BgSprite bg(base, StatMan.getLevel());
	bg.push();

	oil.setPos(35, 45);
	happiness.setPos(35, 65);
	xp.setPos(31, 85);

	base->drawIcon(frameFile, 16, 16, 128, 96);

	base->setTextColor(TFT_BLACK);
	base->setTextDatum(lgfx::textdatum::top_left);
	base->setCursor(35, 28);
	base->print("Stats increased!");

	base->setTextDatum(lgfx::textdatum::top_left);
	base->setCursor(100, 45);
	base->printf("+ %d", stats.oilLevel);
	base->setCursor(100, 65);
	base->printf("+ %d", stats.happiness);
	base->setCursor(100, 85);
	base->printf("+ %d", stats.experience);

	Input::getInstance()->addListener(this);
	LoopManager::resetTime();
	LoopManager::addListener(this);
}

void ScoreScreen::onStop(){
	Input::getInstance()->removeListener(this);
	LoopManager::removeListener(this);
}


void ScoreScreen::buttonPressed(uint i){
	if(easeDone) exit();
}

void ScoreScreen::loop(uint micros){

	if(easeDone && (::micros() - t) > exitTime){
		exit();
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
			currentStats.experience = prevStats.experience + ((float)((int)(targetStats.experience) - (int)(prevStats.experience))) * ease;
		}

		happiness.setLevel(currentStats.happiness);
		oil.setLevel(currentStats.oilLevel);
		xp.setLevel(StatMan.getExpPercentage(currentStats.experience));
	}else if(!easeDone){
		easeDone = true;
		t = ::micros();
	}
	oil.push();
	happiness.push();
	xp.push();
}

void ScoreScreen::exit(){
	StatMan.update(stats);
	State::pop();
}
