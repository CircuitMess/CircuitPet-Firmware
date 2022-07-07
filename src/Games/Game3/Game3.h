
#ifndef CIRCUITPET_FIRMWARE_GAME3_H
#define CIRCUITPET_FIRMWARE_GAME3_H


#include <Input/InputListener.h>
#include "../../GameEngine/Game.h"
#include "Duck.h"

class Game3 : public Game, private InputListener {
public:
	Game3();

protected:
	void onLoad() override;
	void onLoop(float deltaTime) override;
	void onStart() override;
	void onStop() override;
	void onRender(Sprite* canvas) override;

private:
	void buttonPressed(uint i) override;

	struct Item{
		std::shared_ptr<GameObject> gObj;
		bool edible;
		float fallSpeed;
	};

	Duck* duck;
	std::shared_ptr<GameObject> bg;
	std::shared_ptr<GameObject> test;
	std::vector<Item> items;
};


#endif //CIRCUITPET_FIRMWARE_GAME3_H
