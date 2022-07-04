#include "Player.h"

void Player::leftTurn(float delta){
	angle -= delta * rotSpeed;
	angle = fmod(angle, 360.0f);

	//TODO - apply rotation to render component somehow?
}

void Player::rightTurn(float delta){
	angle += delta * rotSpeed;
	angle = fmod(angle, 360.0f);
}

float Player::getAngle() const{
	return angle;
}

void Player::setObj(Player::ObjPtr obj){
	Player::obj = obj;

}

const Player::ObjPtr& Player::getObj() const{
	return obj;
}

