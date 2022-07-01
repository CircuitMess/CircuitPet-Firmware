
#include "OilCan.h"

OilCan::OilCan(std::shared_ptr<Sprite> sprite) : oilCan(sprite){
	fill(0.0f);
}

void OilCan::fill(float fillPercent){
	oilCan->fillRect(0,0,10,15,TFT_RED);
	if(fillPercent >= 1.0f) return;
	oilCan->fillRect(0,0,10,15-(15*fillPercent), TFT_BLACK);
}
