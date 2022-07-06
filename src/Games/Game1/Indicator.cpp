
#include "Indicator.h"

Indicator::Indicator(std::shared_ptr<GameObject> indicator) : indicator(indicator){}


void Indicator::move(float deltaTime){
	speed = std::max(speedMin, speedMax - abs(yPos - yGoal)/100.0f);
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

	yPos = (x2 - indicatorVal) * yBar1 / (x2 - x1) + (x1 - indicatorVal) * yBar2 / (x1 - x2);
	indicator->setPos({ 140, yPos - offset});
}

int Indicator::getDifference(){
	return abs(yPos-yGoal);
}

void Indicator::setGoal(int yGoal){
	this->yGoal = yGoal;
}
