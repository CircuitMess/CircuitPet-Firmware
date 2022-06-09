
#ifndef CIRCUITPET_FIRMWARE_BGSPRITE_H
#define CIRCUITPET_FIRMWARE_BGSPRITE_H


#include <Display/Sprite.h>

class BgSprite {
public:
	BgSprite(Sprite* parent, uint8_t level);
	void setLevel(uint8_t level);
	void push();
private:
	Sprite sprite;

	constexpr static uint8_t w = 160;
	constexpr static uint8_t h = 128;
};


#endif //CIRCUITPET_FIRMWARE_BGSPRITE_H
