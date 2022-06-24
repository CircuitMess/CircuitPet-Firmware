
#include "StatsSprite.h"

StatsSprite::StatsSprite(Sprite* parent, uint8_t battery, uint8_t oilLevel, uint8_t happiness) :
		battery(parent, StatSprite::Battery, battery),
		oilLevel(parent, StatSprite::OilLevel, oilLevel),
		happiness(parent, StatSprite::Happiness, happiness){

	this->battery.setPos(x, y);
	this->oilLevel.setPos(x + spacing, y);
	this->happiness.setPos(x + 2 * spacing, y);
}

void StatsSprite::push(){
	happiness.push();
	oilLevel.push();
	battery.push();
}

void StatsSprite::setHappiness(uint8_t level){
	happiness.setLevel(level);
}

void StatsSprite::setOilLevel(uint8_t level){
	oilLevel.setLevel(level);
}

void StatsSprite::setBattery(uint8_t level){
	battery.setLevel(level);
}
