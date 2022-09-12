#ifndef CIRCUITPET_FIRMWARE_CLOCKLISTENER_H
#define CIRCUITPET_FIRMWARE_CLOCKLISTENER_H

#include <Arduino.h>
#include <functional>

class ClockMaster;

class ClockListener {
friend ClockMaster;

public:
	/**
	 * @param tickInterval Interval after which the function will be called, in seconds.
	 * @param persistent RTC persistency after device is reset. Saves last known time to non-volatile memory.
	 * @param id Character string ID.
	 * @param func Function to be called.
	 */
	ClockListener(time_t tickInterval, bool persistent, const char* id, std::function<void()> func);
	void reset();

private:
	time_t lastTick = 0; //[s]

	const time_t tickInterval;
	const bool persistent; // persists after shutdown
	char ID[10]; //used to identify persistent listeners
	const std::function<void()> func;

};


#endif //CIRCUITPET_FIRMWARE_CLOCKLISTENER_H
