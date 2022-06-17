
#ifndef CIRCUITPET_FIRMWARE_COLLISIONCOMPONENT_H
#define CIRCUITPET_FIRMWARE_COLLISIONCOMPONENT_H

#include "CircleCC.h"
#include "SquareCC.h"
#include <memory>
#include <iostream>

class CollisionComponent {
protected:
	enum Type{Circle, Square} type;
	CollisionComponent(Type type);
public:
	Type getType();
	CircleCC* getCircle();
	SquareCC* getSquare();
};


#endif //CIRCUITPET_FIRMWARE_COLLISIONCOMPONENT_H
