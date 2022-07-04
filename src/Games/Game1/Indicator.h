
#ifndef CIRCUITPET_FIRMWARE_INDICATOR_H
#define CIRCUITPET_FIRMWARE_INDICATOR_H


#include <memory>
#include "../../GameEngine/GameObject.h"

class Indicator {
public:
	Indicator(std::shared_ptr<GameObject> indicator);
	void move(float deltaTime);
	int getYPos();
	void setGoal(int yGoal);
private:
	std::shared_ptr<GameObject> indicator;
	int yPos;
	int yGoal;
	float indicatorVal = 0.0f;
	float speed = 0.0f;
	float speedMin = 1.0f;
	float speedMax = 2.0f;
	bool goingUp = false;
	const float barMax = 1.0f;
	const float x1 = -barMax;
	const float x2 = barMax;
	const int y1 = -2; //4 - (11/2)
	const int y2 = 118; //124 - (11/2)
};


#endif //CIRCUITPET_FIRMWARE_INDICATOR_H
