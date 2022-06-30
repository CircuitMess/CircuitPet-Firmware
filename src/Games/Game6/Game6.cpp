#include <Pins.hpp>
#include "Game6.h"
#include "../../GameEngine/Rendering/AnimRC.h"
#include "../../GameEngine/Rendering/StaticRC.h"
#include "../../GameEngine/Collision/RectCC.h"
#include "../../GameEngine/Collision/CircleCC.h"
#include <Input/Input.h>

Game6::Game6() : Game("", {
		{ "/Bg/Level6.raw", {}, true },
		{ "/MenuIcons/Icon1.raw", {}, true }}){

}

void Game6::onLoad(){
	auto pat = std::make_shared<GameObject>(std::make_unique<StaticRC>(getFile("/MenuIcons/Icon1.raw"), PixelDim{32, 32}),
											std::make_unique<CircleCC>(16));
	addObject(pat);
	player.setObj(pat);

	auto bg = std::make_shared<GameObject>(
			std::make_unique<StaticRC>(getFile("/Bg/Level1.raw"), PixelDim { 160, 128 }),
			nullptr
	);
	addObject(bg);
	bg->getRenderComponent()->setLayer(-1);
}

void Game6::onLoop(float deltaTime){

	if(leftHold && !rightHold){
		player.leftTurn(deltaTime);
	}else if(rightHold && !leftHold){
		player.rightTurn(deltaTime);
	}
}

void Game6::onRender(Sprite* canvas){

}

void Game6::onStart(){
	Input::getInstance()->addListener(this);
}

void Game6::onStop(){
	Input::getInstance()->removeListener(this);
}

void Game6::buttonPressed(uint i){
	switch(i){
		case BTN_BACK:
			pop();
			break;

		case BTN_LEFT:
			leftHold = true;
			break;

		case BTN_RIGHT:
			rightHold = true;
			break;

		case BTN_ENTER:
			Serial.println(player.getAngle());
			break;
	}
}

void Game6::buttonReleased(uint i){
	switch(i){
		case BTN_LEFT:
			leftHold = false;
			break;

		case BTN_RIGHT:
			rightHold = false;
			break;
	}
}
