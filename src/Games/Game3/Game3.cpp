#include <Pins.hpp>
#include "Game3.h"
#include "../../GameEngine/Rendering/SpriteRC.h"
#include "../../GameEngine/Rendering/StaticRC.h"
#include "../../GameEngine/Collision/RectCC.h"

#include <Input/Input.h>

Game3::Game3() : Game("/Games/Game3", {
		{ "/Background.raw", {}, true },
		{ "/DuckWalk.gif",   {}, true },
		{ "/DuckEat.gif",    {}, true }
}){}

void Game3::onLoad(){
	bg = std::make_shared<GameObject>(
			std::make_unique<StaticRC>(getFile("/Background.raw"), PixelDim{ 160, 128 }),
			nullptr
	);
	addObject(bg);
	bg->getRenderComponent()->setLayer(-1);

	duck = new Duck(getFile("/DuckWalk.gif"), getFile("/DuckEat.gif"));
	addObject(duck->getGameObject());

	test = std::make_shared<GameObject>(
			std::make_unique<SpriteRC>(PixelDim{ 10, 10 }),
			std::make_unique<RectCC>(PixelDim{ 10, 10 })
	);
	//Item testItem{test, true};


	collision.addPair(*duck->getGameObject(), *test, nullptr);
}

void Game3::onLoop(float deltaTime){
	duck->loop(deltaTime);
}

void Game3::onStart(){
	Input::getInstance()->addListener(this);
}

void Game3::onStop(){
	Input::getInstance()->removeListener(this);
}

void Game3::onRender(Sprite* canvas){
}

void Game3::buttonPressed(uint i){
	if(i == BTN_BACK){
		pop();
	}

}

