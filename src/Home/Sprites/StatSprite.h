
#ifndef CIRCUITPET_FIRMWARE_STATSPRITE_H
#define CIRCUITPET_FIRMWARE_STATSPRITE_H

#include <Display/Sprite.h>

class StatSprite {
public:
	enum Type {
		Happiness, OilLevel, Battery
	};
	StatSprite(Sprite* parent, Type type, uint8_t level);
	void setLevel(uint8_t level);
	void push();
	void setPos(int16_t x, int16_t y);
private:
	Type type;
	uint8_t level;
	Sprite sprite;


	constexpr static uint8_t iconWidth = 15;
	constexpr static uint8_t iconHeight = 10;
	constexpr static uint8_t barWidth = 24;
	constexpr static uint8_t barHeight = 7;
	static const char* paths[];
	static const char* barPath;

	void draw();
};


#endif //CIRCUITPET_FIRMWARE_STATSPRITE_H
