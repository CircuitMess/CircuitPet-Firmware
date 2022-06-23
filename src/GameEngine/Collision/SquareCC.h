
#ifndef CIRCUITPET_FIRMWARE_SQUARECC_H
#define CIRCUITPET_FIRMWARE_SQUARECC_H
#include "CollisionComponent.h"
#include "../PixelDim.hpp"

class SquareCC : public CollisionComponent{
public:
	SquareCC(PixelDim dim);
	PixelDim getDim() const;
private:
	PixelDim dim;
};


#endif //CIRCUITPET_FIRMWARE_SQUARECC_H
