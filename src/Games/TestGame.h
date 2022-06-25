#ifndef CIRCUITPET_FIRMWARE_TESTGAME_H
#define CIRCUITPET_FIRMWARE_TESTGAME_H

#include <Arduino.h>
#include <Input/InputListener.h>
#include "../GameEngine/Game.h"

class TestGame : public Game, private InputListener {
protected:
public:
	TestGame();

protected:
	void onLoad() override;
	void onLoop(float deltaTime) override;
	void onStart() override;
	void onStop() override;

private:
	struct Object {
		std::shared_ptr<GameObject> gObj;
		glm::vec2 velocity;
	};

	std::vector<Object> objs;

	std::shared_ptr<GameObject> pat;
	std::shared_ptr<GameObject> bg;

	void buttonPressed(uint i) override;

};


#endif //CIRCUITPET_FIRMWARE_TESTGAME_H
