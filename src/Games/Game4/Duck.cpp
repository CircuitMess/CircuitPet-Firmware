
#include "Duck.h"
#include "Game4.h"

Duck::Duck(std::shared_ptr<GameObject> duckGO, Game4* game4) : gameObject(duckGO), game4(game4){
	animRc = std::static_pointer_cast<AnimRC>(duckGO->getRenderComponent());
	animRc->start();
}

void Duck::death(){
	animRc->setAnim(down);
	animRc->setLoopDoneCallback([this](uint32_t){
		Serial.println("DEATH!");
		game4->pop();
	});
}

void Duck::crouch(){

}

void Duck::jump(){

}

std::shared_ptr<GameObject> Duck::getGameObject(){
	return gameObject;
}

void Duck::setFiles(File walk, File down, File preJump, File midJump, File afterJump){
	this->walk = walk;
	this->down = down;
	this->preJump = preJump;
	this->midJump = midJump;
	this->afterJump = afterJump;
}
