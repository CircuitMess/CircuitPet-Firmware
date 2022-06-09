
#ifndef CIRCUITPET_FIRMWARE_STATSPRITE_H
#define CIRCUITPET_FIRMWARE_STATSPRITE_H
#include <Display/Sprite.h>

class StatSprite {
public:
	enum Type{Happiness=0, OilLevel=1, Battery=2};
	StatSprite(Sprite* parent, Type type, uint8_t level);
	void setLevel(uint8_t level);
	void push();
	void setPos(uint8_t x, uint8_t y);
private:
	Type type;
	uint8_t level;
	Sprite sprite;

	constexpr static uint8_t w = 50;
	constexpr static uint8_t h = 10;
	constexpr static uint8_t wBar = 34;
	constexpr static uint8_t offset = h + 5;

	void draw();
};


#endif //CIRCUITPET_FIRMWARE_STATSPRITE_H
