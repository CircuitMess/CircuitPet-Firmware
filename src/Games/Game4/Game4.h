
#ifndef CIRCUITPET_FIRMWARE_GAME4_H
#define CIRCUITPET_FIRMWARE_GAME4_H


#include <Input/InputListener.h>
#include "../../GameEngine/Game.h"
#include "TileManager.h"
#include "../../GameEngine/Rendering/AnimRC.h"
#include "Duck.h"
#include "../../GameEngine/Rendering/SpriteRC.h"

namespace Game4 {
class Game4 : public Game, private InputListener{
public:
	Game4();
	float getSpeed();

protected:
	void onLoad() override;
	void onLoop(float deltaTime) override;
	void onStart() override;
	void onStop() override;
	void onRender(Sprite* canvas) override;

private:
	std::unique_ptr<TileManager> tileManager;
	std::unique_ptr<Duck> duck;

	void buttonPressed(uint i) override;

	struct Obstacle{
		File file;
		PixelDim dim;
		std::initializer_list<glm::vec2> points;
	};
	void setupObstacles();
	void spawn();
	void scoreUp();

	const int tileDim = 16;
	const int topY = 128 - 32;
	const int tilesPerArray = 5;
	float spawnValue = 0.0f;
	float spawnRate = 2.0f;
	const float spawnIncrement = 0.03f;
	float speed = 100.0f;
	const float speedMax = 180.0f;
	const float speedIncrement = 0.5f;
	bool isDone = false;
	int score = 0;
	const int scoreMax = 30;
	int lastIndex = 0;

	std::shared_ptr<Sprite> scoreSprite;
	std::shared_ptr<GameObject> leftWall;
	std::shared_ptr<GameObject> leftWallObject;
	std::shared_ptr<GameObject> bg;
	std::shared_ptr<GameObject> goal;
	std::vector<Obstacle> obstacleUnder;
	std::vector<Obstacle> obstacleOver;
	std::vector<std::shared_ptr<GameObject>> movingObjects;
	std::vector<std::shared_ptr<GameObject>> movingTiles;
};
}

#endif //CIRCUITPET_FIRMWARE_GAME4_H
