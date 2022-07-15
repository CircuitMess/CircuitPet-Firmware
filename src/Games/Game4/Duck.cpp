
#include "Duck.h"

Duck::Duck(std::shared_ptr<GameObject> duckGO) : gameObject(duckGO){
	animRc = std::static_pointer_cast<AnimRC>(duckGO->getRenderComponent());
	animRc->start();
}

void Duck::death(){

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
