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
	 * @param single True - function will be called once, False - function will be called repeatedly,
	 * @param persistent RTC persistency after device is reset. Saves last known time to non-volatile memory.
	 * @param id Character string ID.
	 * @param func Function to be called.
	 */
	ClockListener(time_t tickInterval, bool single, bool persistent, const char* id, std::function<void()> func);
	void reset();

private:
	time_t lastTick = 0; //[s]
	uint32_t lastTickMillis = 0; //[ms] used as a failsafe when listener is triggered immediately after a sync

	const time_t tickInterval;
	const bool single; // event is periodic or triggered only once
	const bool persistent; // persists after shutdown
	char ID[10]; //used to identify persistent listeners
	const std::function<void()> func;

};


#endif //CIRCUITPET_FIRMWARE_CLOCKLISTENER_H
