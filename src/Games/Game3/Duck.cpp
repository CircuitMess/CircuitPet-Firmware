
#include <Pins.hpp>
#include "Duck.h"
#include "../../GameEngine/Rendering/SpriteRC.h"
#include <Input/Input.h>

Duck::Duck(){
	Input::getInstance()->addListener(this);
	go = std::make_shared<GameObject>(
			std::make_unique<SpriteRC>(PixelDim {width, height}),
			nullptr
	);
	go->setPos({(160-width)/2, 128-height-5});
}

Duck::~Duck(){
	Input::getInstance()->removeListener(this);
}

void Duck::loop(float deltaTime){
	float moveX = deltaTime*velocity*speed;
	moveX += go->getPos().x;
	if(moveX < 0 || moveX > 160 - width) return;
	go->setPos({moveX, go->getPos().y});
	Serial.printf("moveX: %d, deltaTime: %f, velocity: %f\n", moveX, deltaTime, velocity);
}

std::shared_ptr<GameObject> Duck::getGameObject(){
	return go;
}

void Duck::buttonPressed(uint i){
	if(i == BTN_LEFT){
		velocity = -1.0f;
	}
	else if(i == BTN_RIGHT){
		velocity = 1.0f;
	}
}

void Duck::buttonReleased(uint i){
	if(i == BTN_LEFT || i == BTN_RIGHT){
		velocity = 0.0f;
	}
}
