
#ifndef CIRCUITPET_FIRMWARE_COLLISIONCOMPONENT_H
#define CIRCUITPET_FIRMWARE_COLLISIONCOMPONENT_H

#include <memory>
#include <iostream>

class SquareCC;

class CircleCC;

enum class CollisionType {
	Circle, Square
};

class CollisionComponent {
protected:
	CollisionType type;
	CollisionComponent(CollisionType type);
public:
	CollisionType getType() const;
	const CircleCC* getCircle() const;
	const SquareCC* getSquare() const;
};


#endif //CIRCUITPET_FIRMWARE_COLLISIONCOMPONENT_H
