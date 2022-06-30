
#ifndef CIRCUITPET_FIRMWARE_GAME1_H
#define CIRCUITPET_FIRMWARE_GAME1_H

#include <Arduino.h>
#include <Input/InputListener.h>
#include "../../GameEngine/Game.h"

class Game1 : public Game, private InputListener {
public:
	Game1();
protected:
	void onLoad() override;
	void onLoop(float deltaTime) override;
	void onStart() override;
	void onStop() override;
	void onRender(Sprite* canvas) override;

private:
	float barValue = 0.0f;
	bool goingUp = false;

	const float barMax = 1.0f;
	const float x1 = -barMax;
	const float x2 = barMax;
	const int y1 = -1; //14 - (11/2)
	const int y2 = 115; //114 - (11/2)
	int yPos;
	int yGoal;

	//Sprite oilCan;
	//Sprite goal;
	std::shared_ptr<GameObject> goal;
	std::shared_ptr<GameObject> indicator;

	//std::shared_ptr<GameObject> duck;
	std::shared_ptr<GameObject> bg;

	void buttonPressed(uint i) override;
	void resetGoal();
};


#endif //CIRCUITPET_FIRMWARE_GAME1_H
