
#include "StatsSprite.h"

StatsSprite::StatsSprite(Sprite* parent, uint8_t happiness, uint8_t oilLevel, uint8_t battery):
	happiness(parent, StatSprite::Happiness, happiness),
	oilLevel(parent, StatSprite::OilLevel, oilLevel),
	battery(parent, StatSprite::Battery, battery){

	this->happiness.setPos(x,y);
	this->oilLevel.setPos(x, y + spacing);
	this->battery.setPos(x, y + 2*spacing);
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
