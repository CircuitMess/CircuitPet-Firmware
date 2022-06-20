
#ifndef CIRCUITPET_FIRMWARE_STATSSPRITE_H
#define CIRCUITPET_FIRMWARE_STATSSPRITE_H

#include <Display/Sprite.h>
#include "StatSprite.h"
class StatsSprite {
public:
	StatsSprite(Sprite* parent, uint8_t happiness, uint8_t oilLevel, uint8_t battery);
	void push();
	void setHappiness(uint8_t level);
	void setOilLevel(uint8_t level);
	void setBattery(uint8_t level);

private:
	StatSprite happiness;
	StatSprite oilLevel;
	StatSprite battery;

	constexpr static uint8_t x = 5;
	constexpr static uint8_t y = 5;
	constexpr static uint8_t spacing = 15;
};


#endif //CIRCUITPET_FIRMWARE_STATSSPRITE_H
