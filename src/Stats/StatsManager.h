#ifndef CIRCUITPET_FIRMWARE_STATSMANAGER_H
#define CIRCUITPET_FIRMWARE_STATSMANAGER_H

#include "StatsChangedListener.h"
#include <Util/WithListeners.h>
#include "Stats.hpp"
#include "../Clock/ClockListener.h"

class StatsManager : public WithListeners<StatsChangedListener> {
public:
	StatsManager();
	void begin();

	void update(Stats delta);

	bool hasDied();

	const Stats& get();
	uint8_t getLevel();

	void setPaused(bool pause);


private:
	void store();
	void load();

	void timedUpdate();


	Stats stats;
	static const uint16_t levelupThresholds[];

	uint8_t gameOverCount = 0;
	bool paused = false;

	ClockListener timedUpdateListener;

	static const Stats hourlyDecrement;
};

extern StatsManager StatMan;
#endif //CIRCUITPET_FIRMWARE_STATSMANAGER_H
