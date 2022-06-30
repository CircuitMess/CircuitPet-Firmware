#ifndef CIRCUITPET_FIRMWARE_GAME6_H
#define CIRCUITPET_FIRMWARE_GAME6_H

#include <Arduino.h>
#include <Input/InputListener.h>
#include "../../GameEngine/Game.h"
#include "Player.h"

class Game6 : public Game, private InputListener {
protected:
public:
	Game6();

protected:
	void onLoad() override;
	void onLoop(float deltaTime) override;
	void onStart() override;
	void onStop() override;
	void onRender(Sprite* canvas) override;

private:
	Player player;

	void buttonPressed(uint i) override;
	void buttonReleased(uint i) override;

	bool leftHold = false;
	bool rightHold = false;

};


#endif //CIRCUITPET_FIRMWARE_GAME6_H
