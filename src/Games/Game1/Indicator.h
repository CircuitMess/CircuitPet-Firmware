
#ifndef CIRCUITPET_FIRMWARE_INDICATOR_H
#define CIRCUITPET_FIRMWARE_INDICATOR_H


#include <memory>
#include "../../GameEngine/GameObject.h"

class Indicator {
public:
	void setPointer(std::shared_ptr<GameObject> indicator);
	void move(float deltaTime);
	int getYPos();
private:
	std::shared_ptr<GameObject> indicator;
	float indicatorVal = 0.0f;
	bool goingUp = false;
	const float barMax = 1.0f;
	const float x1 = -barMax;
	const float x2 = barMax;
	const int y1 = -1; //14 - (11/2)
	const int y2 = 115; //114 - (11/2)
	int yPos;
};


#endif //CIRCUITPET_FIRMWARE_INDICATOR_H
