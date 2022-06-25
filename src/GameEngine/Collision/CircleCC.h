
#ifndef CIRCUITPET_FIRMWARE_CIRCLECC_H
#define CIRCUITPET_FIRMWARE_CIRCLECC_H

#include <cstdint>
#include "CollisionComponent.h"

// TODO: add position offset
class CircleCC : public CollisionComponent{
public:
	CircleCC(float radius);
	float getRadius() const;
private:
	float radius;
};


#endif //CIRCUITPET_FIRMWARE_CIRCLECC_H
