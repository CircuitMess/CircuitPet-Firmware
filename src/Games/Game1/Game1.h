
#ifndef CIRCUITPET_FIRMWARE_GAME1_H
#define CIRCUITPET_FIRMWARE_GAME1_H

#include <Arduino.h>
#include <Input/InputListener.h>
#include "../../GameEngine/Game.h"
#include "Indicator.h"
#include "OilCan.h"
#include "Bar.h"

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
	void buttonPressed(uint i) override;
	void addPoints(int difference);

	Indicator indicator;
	OilCan* oilCan;
	Bar* bar;

	float fillPercent = 0.0f;

	int tries = 6;

	std::shared_ptr<GameObject> indi;

	std::shared_ptr<GameObject> can;
	std::shared_ptr<GameObject> goal;

	//std::shared_ptr<GameObject> duck;
	std::shared_ptr<GameObject> bg;
};


#endif //CIRCUITPET_FIRMWARE_GAME1_H
