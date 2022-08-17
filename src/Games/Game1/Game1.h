
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
	Stats returnStats() override;

	void resetAnim();
private:
	void buttonPressed(uint i) override;
	void addPoints(int difference);

	std::unique_ptr<Indicator> indicator;
	std::unique_ptr<OilCan> oilCan;
	std::unique_ptr<Bar> bar;

	float fillPercent = 0.0f;
	int tries = 0;
	const float length = 120.0f;
	const float maxPoints = 0.35f;
	float multiplier;
	bool done = false;

	std::shared_ptr<AnimRC> duckAnim;
	std::shared_ptr<GameObject> indicatorGO;
	std::shared_ptr<GameObject> oilCanGO;
	std::shared_ptr<GameObject> barGO;
	std::shared_ptr<GameObject> duckGo;
	std::shared_ptr<GameObject> bg;
};


#endif //CIRCUITPET_FIRMWARE_GAME1_H
