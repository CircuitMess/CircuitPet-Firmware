#ifndef CIRCUITPET_FIRMWARE_STATSMANAGER_H
#define CIRCUITPET_FIRMWARE_STATSMANAGER_H

#include "StatsChangedListener.h"
#include <Util/WithListeners.h>
#include "Stats.hpp"
#include "../Clock/ClockListener.h"
#include <nvs.h>

class StatsManager : public WithListeners<StatsChangedListener> {
public:
	StatsManager();
	void begin();
	/**
	 * Resets the stats to starting numbers, used when pet dies or when factory reset occurs.
	 */
	void reset();

	void update(Stats delta);

	bool hasDied() const;

	bool isHatched() const;
	void setHatched(bool hatched);

	const Stats& get() const;
	uint8_t getLevel() const;
	uint8_t getLevel(uint16_t exp) const;
	uint8_t getExpPercentage() const;
	uint8_t getExpPercentage(uint16_t exp) const;

private:
	void store();
	void load();

	void timedUpdate();


	Stats stats;
	static const uint16_t levelupThresholds[];

	static constexpr uint8_t gameOverHours = 24;
	uint8_t gameOverCount = 0;
	bool hatched = false;

	ClockListener timedUpdateListener;

	static const Stats hourlyDecrement;

	static nvs_handle handle;
};

extern StatsManager StatMan;
#endif //CIRCUITPET_FIRMWARE_STATSMANAGER_H
