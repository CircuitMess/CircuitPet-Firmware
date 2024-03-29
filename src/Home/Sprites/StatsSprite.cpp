
#include "StatsSprite.h"
#include "../../Stats/StatsManager.h"

StatsSprite::StatsSprite(Sprite* parent, uint8_t oilLevel, uint8_t happiness, uint8_t battery) :
		oilLevel(parent, StatSprite::OilLevel, oilLevel),
		happiness(parent, StatSprite::Happiness, happiness),
		battery(parent, StatSprite::Battery, battery),
		xpLevel(parent, StatSprite::XPLevel, StatMan.getExpPercentage(), true){
}

void StatsSprite::setPos(int16_t x, int16_t y){
	oilLevel.setPos(x-7,y);
	happiness.setPos(x - 3 + spacing, y);
	battery.setPos(x + 2*spacing + 6, y);
	xpLevel.setPos(x-5,y+9);
}

void StatsSprite::push(){
	oilLevel.push();
	happiness.push();
	battery.push();
	xpLevel.push();
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

void StatsSprite::setXPLevel(){
	xpLevel.setLevel(StatMan.getExpPercentage());
}
