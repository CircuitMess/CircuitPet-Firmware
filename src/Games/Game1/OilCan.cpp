
#include "OilCan.h"


void OilCan::fill(float fillPercent){
	//printf("OilCan fillPercent: %f\n", fillPercent);
	sprite->clear(TFT_TRANSPARENT);
	sprite->drawIcon(fullCan, 0, 0, 24, 21);
	if(fillPercent >= 1.0f) return;
	sprite->drawIcon(emptyCan, 0, 0, 24, 21 - (21 * fillPercent));
}

OilCan::OilCan(std::shared_ptr<Sprite> sprite, File fullCan, File emptyCan) : sprite(sprite), fullCan(fullCan), emptyCan(emptyCan){
	fill(0.0f);
}

