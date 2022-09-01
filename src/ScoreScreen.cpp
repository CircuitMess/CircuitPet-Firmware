
#include "ScoreScreen.h"
#include "Stats/StatsManager.h"
#include "Home/Sprites/BgSprite.h"
#include <CircuitPet.h>
#include <SPIFFS.h>
#include <Input/Input.h>

ScoreScreen::ScoreScreen(Stats stats): stats(stats){
	base = CircuitPet.getDisplay()->getBaseSprite();
}

void ScoreScreen::onStart(){
	File frame = SPIFFS.open("/frame.raw");
	BgSprite bg(base,StatMan.getLevel());
	bg.push();
	base->drawIcon(frame,16,16,128,96);

	base->setTextColor(TFT_BLACK);
	base->setCursor(40, 35);
	base->print("Oil: ");
	base->print(stats.oilLevel);
	base->setCursor(40, 50);
	base->print("Happiness: ");
	base->print(stats.happiness);
	base->setCursor(40, 65);
	base->print("XP: ");
	base->print(stats.experience);

	Input::getInstance()->addListener(this);
}

void ScoreScreen::onStop(){
	Input::getInstance()->removeListener(this);
}


void ScoreScreen::buttonPressed(uint i){
	Input::getInstance()->removeListener(this);
	State::pop();
}
