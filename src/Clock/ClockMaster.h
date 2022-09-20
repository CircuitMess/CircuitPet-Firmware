#ifndef CIRCUITPET_FIRMWARE_CLOCKMASTER_H
#define CIRCUITPET_FIRMWARE_CLOCKMASTER_H

#include <Util/WithListeners.h>
#include <Loop/LoopListener.h>
#include <vector>
#include <map>
#include <FS.h>
#include "ClockListener.h"
#include <string>
#include <nvs.h>

struct PersistentListener{
	char ID[10];
	time_t lastTick;
};

extern ClockMaster Clock;

class ClockMaster : private LoopListener{
public:
	ClockMaster();
	void begin();
	void updatePersistent();
	void loop(uint micros) override;

	void addListener(ClockListener* listener);
	void removeListener(ClockListener* listener);

private:
	std::vector<ClockListener*> listeners;
	std::map <std::string, PersistentListener> persistentListeners;

	void write();
	void read();

	void processTime();

	uint32_t updateTime;
	static constexpr uint32_t updateInterval = 50; // [ms]<
	static constexpr uint32_t updateCount = 50;
	std::vector<time_t> times;

	static nvs_handle handle;
};


#endif //CIRCUITPET_FIRMWARE_CLOCKMASTER_H
