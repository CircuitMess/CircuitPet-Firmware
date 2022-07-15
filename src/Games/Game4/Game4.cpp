#include <Pins.hpp>
#include "Game4.h"
#include "../../GameEngine/Rendering/StaticRC.h"
#include "../../GameEngine/Collision/RectCC.h"
#include "../../GameEngine/Collision/CircleCC.h"
#include <Input/Input.h>

Game4::Game4() : Game("/Games/Game4", {
		{ "/Background.raw",     {}, true },
		{ "/TileTop1.raw",       {}, true },
		{ "/TileTop2.raw",       {}, true },
		{ "/TileBot1.raw",       {}, true },
		{ "/TileBot2.raw",       {}, true },
		{ "/ObstacleOver1.raw",  {}, false },
		{ "/ObstacleOver2.raw",  {}, false },
		{ "/ObstacleOver3.raw",  {}, false },
		{ "/ObstacleOver4.raw",  {}, false },
		{ "/ObstacleUnder1.raw", {}, false },
		{ "/ObstacleUnder2.raw", {}, false },
		{ "/ObstacleUnder3.raw", {}, false },
		{ "/DuckWalk.gif",       {}, false },
		{ "/DuckPreJump.gif",    {}, false },
		{ "/DuckMidJump.gif",    {}, false },
		{ "/DuckAfterJump.gif",  {}, false },
		{ "/DuckCrouch.gif",     {}, false },
		{ "/DuckDown.gif",       {}, false }
}){}

void Game4::onLoad(){
	srand(time(nullptr));

	setupObstacles();

	tileManager = new TileManager(movingObjects);
	tileManager->addFilePair(getFile("/TileTop1.raw"), getFile("/TileBot1.raw"));
	tileManager->addFilePair(getFile("/TileTop2.raw"), getFile("/TileBot2.raw"));
	tileManager->create(); // creates GameObjects into movingObjects

	leftWall = std::make_shared<GameObject>(
			nullptr,
			std::make_unique<RectCC>(glm::vec2{ 1, 128 })
	);
	leftWall->setPos({ -tileDim * tilesPerArray, 0 });
	addObject(leftWall);

	for(auto obj: movingObjects){
		addObject(obj);
		collision.addPair(*leftWall, *obj, [this, obj](){ tileManager->reset(obj); });
	}

	bg = std::make_shared<GameObject>(
			std::make_unique<StaticRC>(getFile("/Background.raw"), PixelDim{ 160, topY }),
			nullptr
	);
	bg->getRenderComponent()->setLayer(-1);
	addObject(bg);

	auto duckGO = std::make_shared<GameObject>(
			std::make_unique<AnimRC>(getFile("/DuckWalk.gif")),
			std::make_unique<CircleCC>(15.0f, glm::vec2{ 20, 20 })
	);
	addObject(duckGO);
	duckGO->getRenderComponent()->setLayer(0);
	duckGO->setPos({ 5, 50 });

	duck = new Duck(duckGO);
	duck->setFiles(getFile("/DuckWalk.gif"),
				   getFile("/DuckDown.gif"),
				   getFile("/DUckPreJump.gif"),
				   getFile("/DuckMidJump.gif"),
				   getFile("/DuckAfterJump.gif"));
}

void Game4::onLoop(float deltaTime){
	for(auto obj: movingObjects){
		float x = obj->getPos().x - deltaTime * speed;
		float y = obj->getPos().y;
		obj->setPos({ x, y });
	}
	value += deltaTime;
	if(value >= spawnRate){
		value -= spawnRate;
		spawn();
	}
}

void Game4::onStart(){
	Input::getInstance()->addListener(this);
}

void Game4::onStop(){
	Game::onStop();
}

void Game4::onRender(Sprite* canvas){
	Game::onRender(canvas);
}

void Game4::buttonPressed(uint i){
	if(i == BTN_BACK){
		pop();
	}
}

void Game4::setupObstacles(){
	obstacleOver.push_back({ getFile("/ObstacleOver1.raw"), { 40, 31 }});
	obstacleOver.push_back({ getFile("/ObstacleOver2.raw"), { 24, 19 }});
	obstacleOver.push_back({ getFile("/ObstacleOver3.raw"), { 22, 19 }});
	obstacleOver.push_back({ getFile("/ObstacleOver4.raw"), { 33, 19 }});

	obstacleUnder.push_back({ getFile("/ObstacleUnder1.raw"), { 36, 27 }});
	obstacleUnder.push_back({ getFile("/ObstacleUnder2.raw"), { 40, 30 }});
	obstacleUnder.push_back({ getFile("/ObstacleUnder3.raw"), { 19, 19 }});
}

void Game4::spawn(){
	int coinFlip = rand() % 2;
	Obstacle obstacle;
	int posY;
	if(coinFlip == 0){
		int under = rand() % obstacleUnder.size();
		obstacle = obstacleUnder[under];
		posY = topY - 40 - obstacle.dim.y;
	}else{
		int over = rand() % obstacleOver.size();
		obstacle = obstacleOver[over];
		posY = topY - obstacle.dim.y;
	}

	auto gObj = std::make_shared<GameObject>(
			std::make_unique<StaticRC>(obstacle.file, obstacle.dim),
			std::make_unique<RectCC>(obstacle.dim)
	);
	addObject(gObj);
	gObj->setPos({ 160, posY });
	gObj->getRenderComponent()->setLayer(1);
	collision.addPair(*duck->getGameObject(), *gObj, [this, gObj](){
//		duckDown(gObj);
	});
	collision.addPair(*leftWall, *gObj, [this, gObj](){ removeObject(gObj); });
	movingObjects.push_back(gObj);
}
//
//void Game4::duckDown(std::shared_ptr<GameObject> gObj){
//	speed = 0.0f;
//	duckAnim->setAnim(getFile("/DuckDown.gif"));
//	collision.removePair(*duck, *gObj);
//	duckAnim->setLoopDoneCallback([this](uint32_t t){ pop(); });
//}
