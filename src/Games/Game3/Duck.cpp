
#include <Pins.hpp>
#include "Duck.h"
#include "../../GameEngine/Collision/CircleCC.h"
#include <Input/Input.h>

Duck::Duck(File walk, File eat, File eatBad, File win) : walk(walk), eat(eat), eatBad(eatBad), win(win){
	Input::getInstance()->addListener(this);
	go = std::make_shared<GameObject>(
			std::make_unique<AnimRC>(this->walk),
			std::make_unique<CircleCC>(beakRadius, glm::vec2{width/2, beakRadius +12 })
	);
	go->setPos({(160-width)/2, 128-height});
	anim =  std::static_pointer_cast<AnimRC>(go->getRenderComponent());
	anim->start();
}

Duck::~Duck(){
	Input::getInstance()->removeListener(this);
}

void Duck::loop(float deltaTime){
	if(eating) return;
	float moveX = deltaTime*velocity*speed;
	moveX += go->getPos().x;
	if(moveX < - width/4 || moveX > 160 - (width*3)/4) return;
	go->setPos({moveX, go->getPos().y});
}

void Duck::startEating(int value){
	eating = true;
	if(value > 0){
		anim->setAnim(eat);
	}else{
		anim->setAnim(eatBad);
	}

	anim->setLoopDoneCallback([this](uint32_t t){
		finishEating();
	});
}

void Duck::finishEating(){
	anim->setAnim(walk);
	anim->setLoopDoneCallback({});
	eating = false;
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

void Duck::filled(){
	anim->setAnim(win);
	anim->setLoopDoneCallback({});
}
