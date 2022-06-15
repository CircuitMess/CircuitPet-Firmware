
#ifndef CIRCUITPET_FIRMWARE_CIRCLECC_H
#define CIRCUITPET_FIRMWARE_CIRCLECC_H

#include <cstdint>

class CircleCC {
public:
	CircleCC(uint16_t radius);
	uint16_t getRadius();
private:
	uint16_t radius;
};


#endif //CIRCUITPET_FIRMWARE_CIRCLECC_H
