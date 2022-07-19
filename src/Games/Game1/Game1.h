
#ifndef CIRCUITPET_FIRMWARE_GAME1_H
#define CIRCUITPET_FIRMWARE_GAME1_H

#include <Arduino.h>
#include <Input/InputListener.h>
#include "../../GameEngine/Game.h"
#include "../../GameEngine/Rendering/AnimRC.h"
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

	void resetAnim();
private:
	void buttonPressed(uint i) override;
	void addPoints(int difference);

	Indicator* indicator;
	OilCan* oilCan;
	Bar* bar;

	float fillPercent = 0.0f;
	int tries = 6;
	const float length = 120.0f;
	const float maxPoints = 0.35f;
	float multiplier;

	std::shared_ptr<AnimRC> duckAnim;
	std::shared_ptr<GameObject> indicatorGO;
	std::shared_ptr<GameObject> oilCanGO;
	std::shared_ptr<GameObject> barGO;
	std::shared_ptr<GameObject> duckGo;
	std::shared_ptr<GameObject> bg;
};


#endif //CIRCUITPET_FIRMWARE_GAME1_H
