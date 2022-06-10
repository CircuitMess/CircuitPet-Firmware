
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

	constexpr static uint8_t width = 50;
	constexpr static uint8_t height = 10;
	constexpr static uint8_t barWidth = 34;
	constexpr static uint8_t barOffset = height + 5;
	constexpr static const char* paths[] = {
			"/Stats/Happiness.raw",
			"/Stats/OilLevel.raw",
			"/Stats/Batter.raw"
	};

	void draw();
};


#endif //CIRCUITPET_FIRMWARE_STATSPRITE_H
