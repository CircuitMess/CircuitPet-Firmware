#ifndef CIRCUITPET_FIRMWARE_CLOCKMASTER_H
#define CIRCUITPET_FIRMWARE_CLOCKMASTER_H

#include <Util/WithListeners.h>
#include <Loop/LoopListener.h>
#include <vector>
#include <map>
#include <FS.h>
#include "ClockListener.h"
#include <string>

struct PersistentListener{
	char ID[10];
	time_t lastTick;
};

extern ClockMaster Clock;

class ClockMaster : private LoopListener{
public:
	void begin();
	void loop(uint micros) override;

	void addListener(ClockListener* listener);
	void removeListener(ClockListener* listener);

private:
	std::vector<ClockListener*> listeners;
	std::map <std::string, PersistentListener> persistentListeners;

	File storage;
	File backup;
	void write();
	void read();

	time_t lastRTCTime = 0;

	time_t syncTime();

	uint32_t syncTimeMicros = 0;
	const uint32_t syncTimeInterval = 60000000; //[us]

};


#endif //CIRCUITPET_FIRMWARE_CLOCKMASTER_H
