#ifndef CIRCUITPET_FIRMWARE_PLAYER_H
#define CIRCUITPET_FIRMWARE_PLAYER_H

#include "../../GameEngine/GameObject.h"

class Player{
	typedef std::shared_ptr<GameObject> ObjPtr;

public:
	void setObj(ObjPtr obj);
	void leftTurn(float delta);
	void rightTurn(float delta);

	float getAngle() const;
private:
	float angle = 0.f; //0 - 360

	constexpr static float rotSpeed = 180;

	ObjPtr obj;
};


#endif //CIRCUITPET_FIRMWARE_PLAYER_H
