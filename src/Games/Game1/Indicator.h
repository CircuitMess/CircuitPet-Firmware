
#ifndef CIRCUITPET_FIRMWARE_INDICATOR_H
#define CIRCUITPET_FIRMWARE_INDICATOR_H


#include <memory>
#include "../../GameEngine/GameObject.h"

class Indicator {
public:
	Indicator(std::shared_ptr<GameObject> indicator);
	void move(float deltaTime);
	int getDifference();
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
	const int yBar1 = 1;
	const int yBar2 = 119;
	const int offset = 1; //yBar1 + 5 - 6 // 5 is offset from top, 6 is offset to indicator
};


#endif //CIRCUITPET_FIRMWARE_INDICATOR_H
