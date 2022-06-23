
#ifndef CIRCUITPET_FIRMWARE_CIRCLECC_H
#define CIRCUITPET_FIRMWARE_CIRCLECC_H

#include <cstdint>
#include "CollisionComponent.h"

class CircleCC : public CollisionComponent{
public:
	CircleCC(uint16_t radius);
	uint16_t getRadius() const;
private:
	uint16_t radius;
};


#endif //CIRCUITPET_FIRMWARE_CIRCLECC_H
