
#include "Indicator.h"

Indicator::Indicator(std::shared_ptr<GameObject> indicator) : indicator(indicator){}


void Indicator::move(float deltaTime){
	speed = std::max(speedMin, speedMax - abs(yPos - yGoal)/50.0f);
	printf("Indicator: diff: %f speed: %f\n",abs(yPos - yGoal)/50.0f, speed);
	if(goingUp){
		indicatorVal -= deltaTime * speed;
	}else{
		indicatorVal += deltaTime * speed;
	}
	if(indicatorVal >= barMax){
		indicatorVal = 2 * barMax - indicatorVal;
		goingUp = true;
	}else if(indicatorVal <= -barMax){
		indicatorVal = -2 * barMax - indicatorVal;
		goingUp = false;
	}

	yPos = (x2 - indicatorVal) * y1 / (x2 - x1) + (x1 - indicatorVal) * y2 / (x1 - x2);
	indicator->setPos({ 140, yPos });
}

int Indicator::getYPos(){
	return yPos;
}

void Indicator::setGoal(int yGoal){
	this->yGoal = yGoal;
}
