#include <Pins.hpp>
#include "Game3.h"
#include "../../GameEngine/Rendering/SpriteRC.h"
#include "../../GameEngine/Rendering/StaticRC.h"
#include <Input/Input.h>

Game3::Game3() : Game("",{
		{"/Bg/Level3.raw", {}, true}
}){}

void Game3::onLoad(){
	bg = std::make_shared<GameObject>(
			std::make_unique<StaticRC>(getFile("/Bg/Level3.raw"), PixelDim { 160, 128 }),
			nullptr
	);
	addObject(bg);
	bg->getRenderComponent()->setLayer(-1);

	duckGO = std::make_shared<GameObject>(
			std::make_unique<SpriteRC>(PixelDim {40, 60}),
			nullptr
	);
	addObject(duckGO);
	duckGO->setPos({60, 60});
}

void Game3::onLoop(float deltaTime){
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
