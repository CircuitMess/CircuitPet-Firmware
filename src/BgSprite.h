
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

	constexpr static uint8_t width = 160;
	constexpr static uint8_t height = 128;
};


#endif //CIRCUITPET_FIRMWARE_BGSPRITE_H
