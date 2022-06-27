
#ifndef CIRCUITPET_FIRMWARE_GAME1_H
#define CIRCUITPET_FIRMWARE_GAME1_H


#include "../../GameEngine/Game.h"

class Game1 : public Game {
public:
	Game1();
protected:
	void onLoad() override;
	void onLoop(float deltaTime) override;
	void onStart() override;
	void onStop() override;

private:
	float barValue = 0.0f;
	bool goingUp = false;

	Sprite indicator;
	Sprite oilCan;
	Sprite fillBar;

	std::shared_ptr<GameObject> duck;
	std::shared_ptr<GameObject> bg;
};


#endif //CIRCUITPET_FIRMWARE_GAME1_H
