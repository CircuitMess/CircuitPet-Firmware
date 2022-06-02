#ifndef CIRCUITPET_FIRMWARE_CLOCKLISTENER_H
#define CIRCUITPET_FIRMWARE_CLOCKLISTENER_H

#include <Arduino.h>
#include <functional>

class ClockMaster;

class ClockListener {
friend ClockMaster;

public:
	ClockListener(uint64_t tickInterval, bool single, bool persistent, const char* id, std::function<void()> func);
	void reset();

private:
	uint64_t lastTick = 0; //[ms]
	uint32_t lastTickMillis = 0; //[ms] used as a failsafe when listener is triggered immediately after a sync

	const uint64_t tickInterval;
	const bool single; // event is periodic or triggered only once
	const bool persistent; // persists after shutdown
	char ID[10]; //used to identify persistent listeners
	const std::function<void()> func;

};


#endif //CIRCUITPET_FIRMWARE_CLOCKLISTENER_H
