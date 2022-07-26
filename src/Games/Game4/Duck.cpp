#include <Pins.hpp>
#include "Duck.h"
#include "Game4.h"
#include <Input/Input.h>

Duck::Duck(std::shared_ptr<GameObject> duckGO, Game4* game4) : gameObject(duckGO), game4(game4){
	animRc = std::static_pointer_cast<AnimRC>(gameObject->getRenderComponent());
	circleCc = std::static_pointer_cast<CircleCC>(gameObject->getCollisionComponent());
	animRc->start();
	Input::getInstance()->addListener(this);
}

Duck::~Duck(){
	Input::getInstance()->removeListener(this);
}

void Duck::update(float deltaTime){
	if(!isJumping) return;
	float y = gameObject->getPos().y;
	y += velocity * deltaTime;
	velocity += gravity * deltaTime * multiplier;
	Serial.printf("y = %f\tvelocity = %f\tdeltaTime = %f\n", y, velocity, deltaTime);
	gameObject->setPos({ 5, y });
	if(y > startPosY){
		gameObject->setPos({5,startPosY});
		multiplier = 1.0f;
		isJumping = false;
		walk();
	}
}

void Duck::buttonPressed(uint i){
	if(i == BTN_DOWN){
		if(isJumping){
			multiplier = 10.0f;
		}else{
			animRc->setAnim(ducking);
			animRc->setLoopDoneCallback([this](uint32_t){
				animRc->setAnim(ducked);
			});
			circleCc->setOffset({ 30, 30 });
		}
	}
	if(i == BTN_UP){
		if(isJumping) return;
		isJumping = true;
		velocity = jumpVelocity;
		walk();
		animRc->setAnim(jump);
		animRc->setLoopMode(GIF::Single);
	}
}

void Duck::buttonReleased(uint i){
	if(i == BTN_DOWN){
		if(isJumping) return;
		walk();
	}
}

void Duck::walk(){
	animRc->setLoopMode(GIF::Infinite);
	animRc->setAnim(unDucking);
	animRc->setLoopDoneCallback([this](uint32_t){
		animRc->setAnim(walking);
	});
	circleCc->setOffset({ 25, 20 });
}

void Duck::death(){
	Input::getInstance()->removeListener(this);
	animRc->setAnim(down);
	animRc->setLoopDoneCallback([this](uint32_t){
		animRc->stop();
		game4->pop();
	});
}

std::shared_ptr<GameObject> Duck::getGameObject(){
	return gameObject;
}

void Duck::setFiles(File walk, File down, File jump, File ducking, File ducked, File unDucking){
	this->walking = walk;
	this->down = down;
	this->jump = jump;
	this->ducking = ducking;
	this->ducked = ducked;
	this->unDucking = unDucking;
}
