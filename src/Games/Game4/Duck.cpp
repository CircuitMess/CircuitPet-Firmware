
#include "Duck.h"
#include "Game4.h"

Duck::Duck(std::shared_ptr<GameObject> duckGO, Game4* game4) : gameObject(duckGO), game4(game4){
	animRc = std::static_pointer_cast<AnimRC>(gameObject->getRenderComponent());
	circleCc = std::static_pointer_cast<CircleCC>(gameObject->getCollisionComponent());
	animRc->start();
}

void Duck::death(){
	animRc->setAnim(down);
	animRc->setLoopDoneCallback([this](uint32_t){
		game4->pop();
	});
}

void Duck::crouch(){
	circleCc->setOffset({20,30});
	animRc->setAnim(crouching);
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

void Duck::setFiles(File walk, File down, File preJump, File midJump, File afterJump, File crouching){
	this->walking = walk;
	this->down = down;
	this->preJump = preJump;
	this->midJump = midJump;
	this->afterJump = afterJump;
	this->crouching = crouching;
}
