
#ifndef CIRCUITPET_FIRMWARE_SQUARECC_H
#define CIRCUITPET_FIRMWARE_SQUARECC_H
#include "../PixelDim.h"

class SquareCC {
public:
	SquareCC(PixelDim dim);
	PixelDim getDim();
private:
	PixelDim dim;
};


#endif //CIRCUITPET_FIRMWARE_SQUARECC_H
