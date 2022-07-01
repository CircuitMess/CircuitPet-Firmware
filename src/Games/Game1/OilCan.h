
#ifndef CIRCUITPET_FIRMWARE_OILCAN_H
#define CIRCUITPET_FIRMWARE_OILCAN_H
#include <Display/Sprite.h>

class OilCan {
public:
	OilCan(std::shared_ptr<Sprite> sprite);
	void fill(float fillPercent);
private:
	std::shared_ptr<Sprite> oilCan;
};


#endif //CIRCUITPET_FIRMWARE_OILCAN_H
