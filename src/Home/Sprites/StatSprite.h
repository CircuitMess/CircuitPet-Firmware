
#ifndef CIRCUITPET_FIRMWARE_STATSPRITE_H
#define CIRCUITPET_FIRMWARE_STATSPRITE_H

#include <Display/Sprite.h>

class StatSprite {
public:
	enum Type {
		Happiness, OilLevel, Battery, XPLevel
	};
	StatSprite(Sprite* parent, Type type, uint8_t level, bool longBar = false);
	void setLevel(uint8_t level);
	void push();
	void setPos(int16_t x, int16_t y);
private:
	Type type;
	uint8_t level;
	Sprite sprite;

	bool longBar;


	constexpr static uint8_t iconWidth = 15;
	constexpr static uint8_t XPiconWidth = 19;
	constexpr static uint8_t iconHeight = 10;
	constexpr static uint8_t barWidth = 24;
	constexpr static uint8_t longBarWidth = 97;
	constexpr static uint8_t barHeight = 7;
	static const char* paths[];
	static const char* barPath;

	void drawLevel(uint8_t length);
	void drawLongLevel();
};


#endif //CIRCUITPET_FIRMWARE_STATSPRITE_H
