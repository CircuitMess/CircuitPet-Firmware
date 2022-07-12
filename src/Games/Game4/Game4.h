
#ifndef CIRCUITPET_FIRMWARE_GAME4_H
#define CIRCUITPET_FIRMWARE_GAME4_H


#include <Input/InputListener.h>
#include "../../GameEngine/Game.h"

class Game4 : public Game, private InputListener{
public:
	Game4();

protected:
	void onLoad() override;
	void onLoop(float deltaTime) override;
	void onStart() override;
	void onStop() override;
	void onRender(Sprite* canvas) override;

private:
	void buttonPressed(uint i) override;


	std::shared_ptr<GameObject> leftWall;
	std::shared_ptr<GameObject> objectDuck;
	std::shared_ptr<GameObject> bg;
	std::shared_ptr<GameObject> duck;
	std::vector<std::shared_ptr<GameObject>> movingObjects;

	const int tileDim = 16;
	const float speed = 20.0f;
};


#endif //CIRCUITPET_FIRMWARE_GAME4_H
