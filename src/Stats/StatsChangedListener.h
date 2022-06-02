#ifndef CIRCUITPET_FIRMWARE_STATSCHANGEDLISTENER_H
#define CIRCUITPET_FIRMWARE_STATSCHANGEDLISTENER_H

#include "Stats.hpp"

class StatsManager;

class StatsChangedListener {
	friend StatsManager;

private:
	virtual void statsChanged(const Stats& stats, bool leveledUp) = 0;
};

#endif //CIRCUITPET_FIRMWARE_STATSCHANGEDLISTENER_H
