
#include "OilCan.h"


OilCan::OilCan(std::shared_ptr<Sprite> sprite, File fullCan, File emptyCan) :
		sprite(sprite), fullCan(fullCan), emptyCan(emptyCan){
	fill(0.0f);
}

void OilCan::fill(float fillPercent){
	sprite->clear(TFT_TRANSPARENT);
	sprite->drawIcon(fullCan, 0, 0, 24, 21);
	if(fillPercent >= 1.0f) return;
	sprite->drawIcon(emptyCan, 0, 0, 24, 21 - (21 * fillPercent));
}

bool OilCan::move(float deltaTime){
	if(!moving) return false;
	startPos.y += deltaTime*speed;
	gameObject->setPos(startPos);
	if(startPos.y >= endPos.y){
		moving = false;
		gameObject->setPos(endPos);
		sprite->clear(TFT_TRANSPARENT);
		return true;
	}else{
		return false;
	}
}

void OilCan::setGameObject(std::shared_ptr<GameObject> gameObj){
	gameObject = gameObj;
	startPos = gameObject->getPos();
}

void OilCan::startMoving(){
	moving = true;
}
