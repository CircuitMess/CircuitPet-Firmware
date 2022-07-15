#include <Pins.hpp>
#include "Game4.h"
#include "../../GameEngine/Rendering/StaticRC.h"
#include "../../GameEngine/Rendering/AnimRC.h"
#include "../../GameEngine/Collision/RectCC.h"
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

	for(auto obj : movingObjects){
		addObject(obj);
		collision.addPair(*leftWall, *obj, [this, obj](){
			tileManager->reset(obj);
		});
	}

	bg = std::make_shared<GameObject>(
			std::make_unique<StaticRC>(getFile("/Background.raw"), PixelDim{ 160, 128 }),
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