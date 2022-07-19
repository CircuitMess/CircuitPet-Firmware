
#include "OilCan.h"


void OilCan::fill(float fillPercent){
	sprite->clear(TFT_TRANSPARENT);
	sprite->drawIcon(fullCan, 0, 0, 24, 21);
	if(fillPercent >= 1.0f) return;
	sprite->drawIcon(emptyCan, 0, 0, 24, 21 - (21 * fillPercent));
}

OilCan::OilCan(std::shared_ptr<Sprite> sprite, File fullCan, File emptyCan) :
				sprite(sprite), fullCan(fullCan), emptyCan(emptyCan){
	fill(0.0f);
}

void OilCan::move(float deltaTime){
	if(!moving) return;
	Serial.printf("aaa %f\n", deltaTime);
	startPos.y += deltaTime*speed;
	gameObject->setPos(startPos);
	if(startPos.y >= endPos.y){
		sprite->clear(TFT_TRANSPARENT);
		moving = false;
	}
}

void OilCan::setGameObject(std::shared_ptr<GameObject> gameObj){
	gameObject = gameObj;
	startPos = gameObject->getPos();
}

void OilCan::startMoving(){
	moving = true;
}
