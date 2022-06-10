
#ifndef CIRCUITPET_FIRMWARE_STATSPRITE_H
#define CIRCUITPET_FIRMWARE_STATSPRITE_H
#include <Display/Sprite.h>

class StatSprite {
public:
	enum Type{Happiness, OilLevel, Battery};
	StatSprite(Sprite* parent, Type type, uint8_t level);
	void setLevel(uint8_t level);
	void push();
	void setPos(uint8_t x, uint8_t y);
private:
	Type type;
	uint8_t level;
	Sprite sprite;

	constexpr static uint8_t width = 50;
	constexpr static uint8_t height = 10;
	constexpr static uint8_t barWidth = 34;
	constexpr static uint8_t barOffset = height + 5;
	static const char* paths[];

	void draw();
};


#endif //CIRCUITPET_FIRMWARE_STATSPRITE_H
