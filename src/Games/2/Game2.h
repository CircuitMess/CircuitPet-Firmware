#ifndef CIRCUITPET_FIRMWARE_GAME2_H
#define CIRCUITPET_FIRMWARE_GAME2_H

#include <Arduino.h>
#include <Input/InputListener.h>
#include "../../GameEngine/Game.h"

class Game2 : public Game, private InputListener {
	typedef std::shared_ptr<GameObject> ObjPtr;
public:
	Game2();

protected:
	void onLoad() override;
	void onLoop(float deltaTime) override;
	void onStart() override;
	void onStop() override;
	void onRender(Sprite* canvas) override;

private:
	void gameWin();
	void gameOver();

	void updateDuck(float delta);
	ObjPtr duck;
	static constexpr float defaultSpeed = 50.0f;
	static constexpr float flapSpeed = 50.0f;
	static constexpr float gravity = 130.0f;

	static constexpr float startingY = 59.0f;
	static constexpr float startingX = 30.0f;

	float duckSpeed = 0.f;
	bool firstPress = false;

	void buttonPressed(uint i) override;

};


#endif //CIRCUITPET_FIRMWARE_GAME2_H
