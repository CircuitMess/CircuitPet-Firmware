#ifndef CIRCUITPET_FIRMWARE_GAME2_H
#define CIRCUITPET_FIRMWARE_GAME2_H

#include <Arduino.h>
#include <Input/InputListener.h>
#include "../../GameEngine/Game.h"
#include "../../GameEngine/Rendering/AnimRC.h"
#include "../Common/Hearts.h"
#include "../Common/Score.h"
#include <deque>
#include <glm/vec2.hpp>

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
	Stats returnStats() override;

private:
	static constexpr float gravity = 140.0f;
	float speedX = 60.0f;
	static constexpr float flapSpeedY = 80.0f;
	static constexpr float duckPosX = 30;
	static constexpr float obstacleSpacing = 50;

	enum { FlyIn, Wait, Play, FallOut } state = FlyIn;

	float entry = 0;
	float entryDuration = 1.0f;

	std::unique_ptr<Hearts> hearts;
	std::unique_ptr<Score> scoreDisplay;

	uint8_t life = 3;
	void die();

	ObjPtr duck;
	std::shared_ptr<AnimRC> anim;
	glm::vec2 velocity = { speedX, 0 };

	int score = 0;
	static constexpr uint8_t scoreCutoff = 50;

	struct ObstaclePair {
		ObjPtr top;
		ObjPtr bot;
		bool passed;
	};
	std::deque<ObstaclePair> obstacles;

	void updateDuck(float delta);
	void updateObstacles(float delta);

	void createObstaclePair();
	void resetDuck();

	void buttonPressed(uint i) override;

	struct ObstacleDesc {
		const char* path;
		PixelDim dim;
		std::initializer_list<glm::vec2> collision;
	};

	static constexpr ObstacleDesc TopObstacles[] = {
			{ "/up1.raw", { 50, 27 }, {{0, 10}, {5, 3}, {43, 0}, {49, 6}, {49, 18}, {43, 23}, {32, 26}, {14, 26}, {0, 15}}},
			{ "/up2.raw", { 44, 19 }, {{0, 10}, {9, 4}, {43, 0}, {43, 7}, {16, 18}, {10, 18}}},
			{ "/up3.raw", { 43, 25 }, {{0, 0}, {32, 0}, {42, 11}, {22, 24}, {4, 24}}},
			{ "/up4.raw", { 45, 24 }, {{0, 5}, {11, 0}, {44, 3}, {39, 11}, {10, 23}, {0, 15}}}
	};

	static constexpr ObstacleDesc BotObstacles[] = {
			{ "/down1.raw", { 18, 61 }, {{0, 6}, {9, 0}, {12, 0}, {17, 4}, {17, 60}, {0, 60}}},
			{ "/down2.raw", { 19, 58 }, {{0, 4}, {7, 0}, {11, 0}, {17, 5}, {17, 11}, {10, 36}, {7, 36}, {0, 10}}},
			{ "/down3.raw", { 25, 64 }, {{3, 7}, {11, 0}, {12, 0}, {20, 7}, {24, 63}, {0, 63}}},
			{ "/down4.raw", { 20, 62 }, {{1, 0}, {18, 0}, {19, 61}, {0, 61}}},
			{ "/down5.raw", { 18, 63 }, {{7, 0}, {17, 0}, {14, 62}, {0, 62}, {3, 12}}}
	};
};


#endif //CIRCUITPET_FIRMWARE_GAME2_H
