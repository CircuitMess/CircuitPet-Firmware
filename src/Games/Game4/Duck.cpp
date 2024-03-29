#include <Pins.hpp>
#include "Duck.h"
#include "Game4.h"
#include <Input/Input.h>
#include <CircuitPet.h>

Game4::Duck::Duck(std::shared_ptr<GameObject> duckGoRc, std::shared_ptr<GameObject> duckGoCc, Game4* game4) : gameObjectRc(duckGoRc), gameObjectCc(duckGoCc),
																									   game4(game4){
	animRc = std::static_pointer_cast<AnimRC>(gameObjectRc->getRenderComponent());
	animRc->start();
	Input::getInstance()->addListener(this);
}

Game4::Duck::~Duck(){
	Input::getInstance()->removeListener(this);
	animRc->stop();
}

void Game4::Duck::update(float deltaTime){
	updateInvincibility(deltaTime);
	if(isDone){
		float x = gameObjectRc->getPos().x;
		x += 30.0f * deltaTime;
		gameObjectRc->setPos({ x, gameObjectRc->getPos().y });
		if(x >= 52){
			walk();
			isDone = false;
			gameObjectRc->setPos({ 62, startPosY - 15 });
			animRc->setAnim(up);
			animRc->setLoopDoneCallback([this](uint32_t){
				delay(500);
				game4->pop();
			});
		}
	}
	if(!isJumping) return;
	float y = gameObjectRc->getPos().y;
	y += velocity * deltaTime + 0.5f * gravity * pow(deltaTime, 2);
	velocity += gravity * deltaTime * multiplier;

	if(time < peakTime && !isDead){
		time += deltaTime;
	}else{
		multiplier = 5;
	}

	gameObjectRc->setPos({ 5, y });
	gameObjectCc->setPos({ 5, y });
	if(y > startPosY){
		gameObjectRc->setPos({ 5, startPosY });
		gameObjectCc->setPos({ 5, startPosY });
		multiplier = 1.0f;
		isJumping = false;
		time = 0.0f;
		if(isDead){
			return;
		}
		if(Input::getInstance()->getButtonState(BTN_RIGHT)){
			jump();
		}else if(Input::getInstance()->getButtonState(BTN_LEFT)){
			duck();
		}else{
			animRc->setAnim(walking);
			animRc->setLoopMode(GIF::Infinite);
		}
	}
}

void Game4::Duck::buttonPressed(uint i){
	if(isDone) return;

	if(i == BTN_LEFT){
		Audio.play({{500, 400, 100}});
		if(isJumping){
			multiplier = 10.0f;
		}else{
			duck();
		}
	}
	if(i == BTN_RIGHT){
		jump();
	}
}

void Game4::Duck::buttonReleased(uint i){
	if(isDone) return;

	if(i == BTN_LEFT){
		if(isJumping) return;
		walk();
	}
}

void Game4::Duck::walk(){
	if(!isDucked) return;
	isDucked = false;

	animRc->setAnim(unDucking);
	animRc->setLoopDoneCallback([this](uint32_t){
		animRc->setAnim(walking);
		animRc->setLoopMode(GIF::Infinite);
	});
	gameObjectCc->setRot(0.0f);
}

void Game4::Duck::jump(){
	if(isJumping) return;

	Audio.play({{400, 800, 100}});
	isJumping = true;
	velocity = 2 * maxHeight * game4->getSpeed() / posXForMaxHeight;
	gravity = -2 * maxHeight * pow(game4->getSpeed(), 2) / pow(posXForMaxHeight, 2);
	peakTime = posXForMaxHeight / game4->getSpeed();
	walk();
	animRc->setAnim(jumpAnim);
	animRc->setLoopMode(GIF::Single);
}

void Game4::Duck::duck(){
	if(isDucked) return;
	isDucked = true;

	animRc->setAnim(ducking);
	animRc->setLoopDoneCallback([this](uint32_t){
		animRc->setAnim(ducked);
		animRc->setLoopMode(GIF::Infinite);
	});
	gameObjectCc->setRot(-90.0f);
}

void Game4::Duck::death(){
	isDead = true;
	Input::getInstance()->removeListener(this);
	animRc->setAnim(down);
	animRc->setLoopMode(GIF::Single);
	animRc->setLoopDoneCallback([this](uint32_t){
		animRc->stop();
		delay(1400);
		game4->pop();
	});
}

std::shared_ptr<GameObject> Game4::Duck::getGameObjectCc(){
	return gameObjectCc;
}

void Game4::Duck::setFiles(File walk, File down, File jump, File ducking, File ducked, File unDucking, File up){
	this->walking = walk;
	this->down = down;
	this->jumpAnim = jump;
	this->ducking = ducking;
	this->ducked = ducked;
	this->unDucking = unDucking;
	this->up = up;
}

void Game4::Duck::win(){
	Input::getInstance()->removeListener(this);
	isDone = true;
	if(isDucked){
		walk();
	}
}

void Game4::Duck::updateInvincibility(float delta){
	if(!invincible) return;

	invincibilityTime += delta;

	if((int)(invincibilityTime / invincibilityBlinkDuration) % 2 == 0){
		gameObjectRc->getRenderComponent()->setVisible(false);
	}else{
		gameObjectRc->getRenderComponent()->setVisible(true);
	}

	if(invincibilityTime >= invincibilityDuration){
		invincibilityTime = 0;
		invincible = false;
		gameObjectRc->getRenderComponent()->setVisible(true);
	}
}
