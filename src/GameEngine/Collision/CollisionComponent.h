
#ifndef CIRCUITPET_FIRMWARE_COLLISIONCOMPONENT_H
#define CIRCUITPET_FIRMWARE_COLLISIONCOMPONENT_H

#include <memory>
#include <iostream>

class RectCC;

class CircleCC;

enum class CollisionType {
	Circle, Rect
};

class CollisionComponent {
protected:
	CollisionType type;
	CollisionComponent(CollisionType type);
public:
	virtual ~CollisionComponent() = default;
	CollisionType getType() const;
	const CircleCC* getCircle() const;
	const RectCC* getRect() const;
};


#endif //CIRCUITPET_FIRMWARE_COLLISIONCOMPONENT_H
