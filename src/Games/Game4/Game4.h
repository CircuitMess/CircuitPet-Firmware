
#ifndef CIRCUITPET_FIRMWARE_GAME4_H
#define CIRCUITPET_FIRMWARE_GAME4_H


#include <Input/InputListener.h>
#include "../../GameEngine/Game.h"
#include "TileManager.h"
#include "../../GameEngine/Rendering/AnimRC.h"
#include "Duck.h"

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
	TileManager* tileManager;
	Duck* duck;

	void buttonPressed(uint i) override;
	void setupObstacles();
	void spawn();

//	void duckDown(std::shared_ptr<GameObject> gObj);

	const int tileDim = 16;
	const int topY = 128 - 32;
	const int tilesPerArray = 5;
	const float spawnRate = 3.0f;
	float speed = 20.0f;
	float value = 0.0f;

	struct Obstacle{
		File file;
		PixelDim dim;
	};

	std::shared_ptr<GameObject> leftWall;
	std::shared_ptr<GameObject> bg;
	std::vector<Obstacle> obstacleUnder;
	std::vector<Obstacle> obstacleOver;
	std::vector<std::shared_ptr<GameObject>> movingObjects;
};


#endif //CIRCUITPET_FIRMWARE_GAME4_H
