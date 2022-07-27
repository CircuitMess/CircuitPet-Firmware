#include <Pins.hpp>
#include "Duck.h"
#include "Game4.h"
#include <Input/Input.h>

Duck::Duck(std::shared_ptr<GameObject> duckGoRc, std::shared_ptr<GameObject> duckGoCc, Game4* game4) : gameObjectRc(duckGoRc), gameObjectCc(duckGoCc), game4(game4){
	animRc = std::static_pointer_cast<AnimRC>(gameObjectRc->getRenderComponent());
	animRc->start();
	Input::getInstance()->addListener(this);
}

Duck::~Duck(){
	Input::getInstance()->removeListener(this);
}

void Duck::update(float deltaTime){
	if(!isJumping) return;
	float y = gameObjectRc->getPos().y;
	y += velocity * deltaTime;
	velocity += gravity * deltaTime * multiplier;
	Serial.printf("y = %f\tvelocity = %f\tdeltaTime = %f\n", y, velocity, deltaTime);
	gameObjectRc->setPos({ 5, y });
	gameObjectCc->setPos({ 5, y });
	if(y > startPosY){
		gameObjectRc->setPos({ 5, startPosY});
		gameObjectCc->setPos({ 5, startPosY});
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
			gameObjectCc->setRot(-90.0f);
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
	gameObjectCc->setRot(0.0f);
}

void Duck::death(){
	Input::getInstance()->removeListener(this);
	animRc->setAnim(down);
	animRc->setLoopDoneCallback([this](uint32_t){
		animRc->stop();
		game4->pop();
	});
}

std::shared_ptr<GameObject> Duck::getGameObjectCc(){
	return gameObjectCc;
}

void Duck::setFiles(File walk, File down, File jump, File ducking, File ducked, File unDucking){
	this->walking = walk;
	this->down = down;
	this->jump = jump;
	this->ducking = ducking;
	this->ducked = ducked;
	this->unDucking = unDucking;
}
