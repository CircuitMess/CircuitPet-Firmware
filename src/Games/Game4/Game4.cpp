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
		{ "/ObstacleOver1.raw",  {}, true },
		{ "/ObstacleOver2.raw",  {}, true },
		{ "/ObstacleOver3.raw",  {}, true },
		{ "/ObstacleOver4.raw",  {}, true },
		{ "/ObstacleUnder1.raw", {}, true },
		{ "/ObstacleUnder2.raw", {}, true },
		{ "/ObstacleUnder3.raw", {}, true },
		{ "/DuckDucked.gif",     {}, false },
		{ "/DuckDucking.gif",    {}, false },
		{ "/DuckUnDucking.gif",  {}, false },
		{ "/DuckPreJump.gif",    {}, false },
		{ "/DuckMidJump.gif",    {}, false },
		{ "/DuckAfterJump.gif",  {}, false },
		{ "/DuckDown.gif",       {}, false },
		{ "/DuckWalk.gif",       {}, false }
}){}

Game4::~Game4(){
	delete (duck);
}

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
			std::make_unique<CircleCC>(10.0f, glm::vec2{ 25, 20 })
	);
	addObject(duckGO);
	duckGO->getRenderComponent()->setLayer(0);
	duckGO->setPos({ 5, 50 });

	duck = new Duck(duckGO, this);
	duck->setFiles(getFile("/DuckWalk.gif"),
				   getFile("/DuckDown.gif"),
				   getFile("/DuckPreJump.gif"),
				   getFile("/DuckMidJump.gif"),
				   getFile("/DuckAfterJump.gif"),
				   getFile("/DuckDucking.gif"),
				   getFile("/DuckDucked.gif"),
				   getFile("/DuckUnDucking.gif"));
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
	duck->update(deltaTime);
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

void Game4::buttonReleased(uint i){

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
		posY = topY - 30 - obstacle.dim.y;
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
		speed = 0.0f;
		collision.removePair(*duck->getGameObject(), *gObj);
		duck->death();
	});
	collision.addPair(*leftWall, *gObj, [this, gObj](){ removeObject(gObj); });
	movingObjects.push_back(gObj);
}

