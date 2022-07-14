#ifndef CIRCUITPET_FIRMWARE_GAME2_H
#define CIRCUITPET_FIRMWARE_GAME2_H

#include <Arduino.h>
#include <Input/InputListener.h>
#include "../../GameEngine/Game.h"
#include <deque>

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
	static constexpr float obstacleSpeed = 50.0f;
	static constexpr float flapSpeed = 65.0f;
	static constexpr float gravity = 150.0f;

	static constexpr float startingY = 59.0f;
	static constexpr float startingX = 30.0f;

	GameObject leftWall;
	std::deque<ObjPtr> obstacles;
	void updateObstacles(float delta);

	static constexpr uint16_t minimumDownHeight = 128-30;
	static constexpr uint16_t minimumUpHeight = 30;
	void createObstacleUp();
	void createObstacleDown();

	struct ImageDesc {
		const char* path;
		PixelDim dim;
	};
	static constexpr ImageDesc upObstacles[] = {
			{ "/up1.raw", { 50, 27 }},
			{ "/up2.raw", { 50, 19 }}};

	static constexpr ImageDesc downObstacles[] = {
			{ "/down1.raw", { 18, 61 }},
			{ "/down2.raw", { 19, 58 }},
			{ "/down3.raw", { 25, 64 }},
			{ "/down4.raw", { 20, 62 }},
			{ "/down5.raw", { 18, 63 }}};

	float duckSpeed = 0.f;
	bool firstPress = false;

	void buttonPressed(uint i) override;

};


#endif //CIRCUITPET_FIRMWARE_GAME2_H
