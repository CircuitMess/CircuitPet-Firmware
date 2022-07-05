
#ifndef CIRCUITPET_FIRMWARE_OILCAN_H
#define CIRCUITPET_FIRMWARE_OILCAN_H

#include <Display/Sprite.h>

class OilCan {
public:
	OilCan(std::shared_ptr<Sprite> sprite, File fullCan, File emptyCan);
	void fill(float fillPercent);
private:
	std::shared_ptr<Sprite> sprite;
	File fullCan;
	File emptyCan;
};


#endif //CIRCUITPET_FIRMWARE_OILCAN_H
