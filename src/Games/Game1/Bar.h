
#ifndef CIRCUITPET_FIRMWARE_BAR_H
#define CIRCUITPET_FIRMWARE_BAR_H


#include <Display/Sprite.h>

class Bar {
public:
	Bar(std::shared_ptr<Sprite> sprite, File barFile);
	void resetGoal();
	int getY();
private:
	void draw();
	int yGoal;
	File barFile;
	std::shared_ptr<Sprite> sprite;
};


#endif //CIRCUITPET_FIRMWARE_BAR_H
