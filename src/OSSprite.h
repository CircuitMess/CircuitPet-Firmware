
#ifndef CIRCUITPET_FIRMWARE_OSSPRITE_H
#define CIRCUITPET_FIRMWARE_OSSPRITE_H

#include <Display/Sprite.h>
class OSSprite {
public:
	OSSprite(Sprite* parent, uint8_t level);
	void setLevel(uint8_t level);
	void push();
	void setPos(uint8_t x, uint8_t y);
private:
	Sprite sprite;

	constexpr static uint8_t w = 40;
	constexpr static uint8_t h = 20;
};


#endif //CIRCUITPET_FIRMWARE_OSSPRITE_H
