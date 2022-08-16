
#ifndef CIRCUITPET_FIRMWARE_BAR_H
#define CIRCUITPET_FIRMWARE_BAR_H


#include <Display/Sprite.h>

class Bar {
public:
	Bar(std::shared_ptr<Sprite> sprite);
	void resetGoal();
	int getY();
private:
	void draw();
	uint8_t yGoal;
	const uint8_t width = 9;
	const uint8_t height = 120;
	std::shared_ptr<Sprite> sprite;
};


#endif //CIRCUITPET_FIRMWARE_BAR_H
