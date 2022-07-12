#include <Pins.hpp>
#include "Game4.h"
#include "../../GameEngine/Rendering/StaticRC.h"
#include "../../GameEngine/Rendering/AnimRC.h"
#include "../../GameEngine/Collision/RectCC.h"
#include <Input/Input.h>

Game4::Game4() : Game("/Games/Game4", {
		{ "/Background.raw",  {}, true },
		{ "/TileTop1.raw",    {}, true },
		{ "/TileTop2.raw",    {}, true },
		{ "/TileBot1.raw",    {}, true },
		{ "/TileBot2.raw",    {}, true },
		{ "/ObjectJump1.raw", {}, true },
		{ "/ObjectJump2.raw", {}, true },
		{ "/ObjectJump3.raw", {}, true },
		{ "/ObjectJump4.raw", {}, true },
		{ "/ObjectDuck1.raw", {}, true },
		{ "/ObjectDuck2.raw", {}, true },
		{ "/ObjectDuck3.raw", {}, true },
		{ "/DuckIdle.gif",    {}, false }
}){}

void Game4::onLoad(){
	bg = std::make_shared<GameObject>(
			std::make_unique<StaticRC>(getFile("/Background.raw"), PixelDim{ 160, 128 }),
			nullptr
	);
	bg->getRenderComponent()->setLayer(-1);
	addObject(bg);

	duck = std::make_shared<GameObject>(
			std::make_unique<AnimRC>(getFile("/DuckIdle.gif")),
			std::make_unique<RectCC>(glm::vec2{ 53, 87 })
	);
	addObject(duck);
	duck->getRenderComponent()->setLayer(0);
	duck->setPos({ 5, 9 });

	objectDuck = std::make_shared<GameObject>(
			std::make_unique<StaticRC>(getFile("/ObjectDuck1.raw"), PixelDim{ 36, 27 }),
			std::make_unique<RectCC>(glm::vec2{ 36, 27 })
	);
	objectDuck->setPos({ 90, 60 });
	addObject(objectDuck);
	movingObjects.push_back(objectDuck);

	collision.addPair(*duck, *objectDuck, [](){});

	for(int i = 0; i < 11; i++){
		auto tileTop = std::make_shared<GameObject>(
				std::make_unique<StaticRC>(getFile("/TileTop2.raw"), PixelDim{ tileDim, tileDim }),
				std::make_unique<RectCC>(glm::vec2{ tileDim, tileDim })
		);
		tileTop->setPos({ i * tileDim, 96 });
		addObject(tileTop);
		auto tileBot = std::make_shared<GameObject>(
				std::make_unique<StaticRC>(getFile("/TileBot1.raw"), PixelDim{ tileDim, tileDim }),
				std::make_unique<RectCC>(glm::vec2{ tileDim, tileDim })
		);
		tileBot->setPos({ i * tileDim, 112 });
		addObject(tileBot);
		movingObjects.push_back(tileTop);
		movingObjects.push_back(tileBot);
	}
}

void Game4::onLoop(float deltaTime){
	for(auto obj : movingObjects){
		int x = obj->getPos().x - deltaTime * speed;
		int y = obj->getPos().y;
		obj->setPos({x,y});
	}
}

void Game4::onStart(){
	reinterpret_cast<AnimRC*>(duck->getRenderComponent().get())->start();
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