#include "Player.h"

void Player::leftTurn(float delta){
	angle += delta * rotSpeed;

	//TODO - apply rotation to render component somehow?
}

void Player::rightTurn(float delta){
	angle -= delta * rotSpeed;
}

float Player::getAngle() const{
	return angle;
}

void Player::setObj(Player::ObjPtr obj){
	Player::obj = obj;
}

