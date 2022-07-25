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

void Duck::death(){
	Input::getInstance()->removeListener(this);
	animRc->setAnim(down);
	animRc->setLoopDoneCallback([this](uint32_t){
		animRc->stop();
		game4->pop();
	});
}

void Duck::buttonPressed(uint i){
	if(i == BTN_DOWN){
		animRc->setAnim(ducking);
		animRc->setLoopDoneCallback([this](uint32_t){
			animRc->setAnim(ducked);
		});
		circleCc->setOffset({ 25, 30 });
	}

}

void Duck::buttonReleased(uint i){
	if(i == BTN_DOWN){
		animRc->setAnim(unDucking);
		animRc->setLoopDoneCallback([this](uint32_t){
			animRc->setAnim(walking);
		});
		circleCc->setOffset({ 20, 20 });
	}
}

void Duck::jump(){
	circleCc->setOffset({20, 10});
	animRc->setAnim(down);
}

void Duck::walk(){
	circleCc->setOffset({20,20});
	animRc->setAnim(walking);
}

std::shared_ptr<GameObject> Duck::getGameObject(){
	return gameObject;
}

void Duck::setFiles(File walk, File down, File preJump, File midJump, File afterJump, File ducking, File ducked, File unDucking){
	this->walking = walk;
	this->down = down;
	this->preJump = preJump;
	this->midJump = midJump;
	this->afterJump = afterJump;
	this->ducking = ducking;
	this->ducked = ducked;
	this->unDucking = unDucking;
}
