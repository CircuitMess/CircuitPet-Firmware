#ifndef CIRCUITPET_FIRMWARE_RGBINDICATOR_H
#define CIRCUITPET_FIRMWARE_RGBINDICATOR_H

#include "Stats/StatsChangedListener.h"
#include <Loop/LoopListener.h>

class RGBIndicator : private LoopListener, private StatsChangedListener{
public:
	void start();
	void stop();

	void loop(uint micros) override;
private:
	void statsChanged(const Stats& stats, bool leveledUp) override;

	constexpr static float easeTime = 0.6f;
	float easeTimer = 0;
	uint8_t currentOil = 0;
	uint8_t targetOil = 0;
	uint8_t prevOil = 0;
};

extern RGBIndicator OilRGBIndicator;


#endif //CIRCUITPET_FIRMWARE_RGBINDICATOR_H
