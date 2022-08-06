#include "ClockMaster.h"
#include <SPIFFS.h>
#include <Loop/LoopManager.h>

ClockMaster Clock;

static const char* tag = "ClockMaster";

void ClockMaster::begin(){
	lastRTCTime = syncTime();
	storage = SPIFFS.open("/clock.bin", "r");
	read();

	LoopManager::addListener(this);
}

void ClockMaster::loop(uint micros){
	syncTimeMicros += micros;

	if(syncTimeMicros >= syncTimeInterval){
		syncTimeMicros = 0;
		uint64_t newTime = syncTime();

		int64_t delta = newTime - lastRTCTime - syncTimeInterval / 1000; // TODO - change if RTC chip's time resolution is larger/smaller
		if(abs(delta) > 0){
			ESP_LOGI(tag, "RTC - millis difference over last 60 seconds: %ld\n", delta);
		}
	}

	uint64_t rtcTime = syncTime();
	uint64_t millisTime = millis();
	bool writeNeeded = false;

	for(auto l : listeners){
		uint64_t delta;
		if(l->persistent){
			if(rtcTime < l->lastTick) continue;
			delta = rtcTime - l->lastTick;
		}else{
			delta = millisTime - l->lastTick;
		}

		if(delta >= l->tickInterval){
			if(l->persistent){
				if(l->lastTickMillis != 0 && millisTime - l->lastTickMillis <= 0.5 * l->tickInterval){
					//ignore tick because of discrepancy between RTC and internal clock, probably because of short tick interval and RTC syncing edge cases
					ESP_LOGW(tag, "RTC - millis difference too great, %s tick ignored\n", l->ID);
					continue;
				}
				persistentListeners[l->ID].lastTick = l->lastTick = rtcTime;
				writeNeeded = true;
			}

			for(int i = 0; i < (delta / l->tickInterval); i++){
				l->func();
			}

			l->lastTickMillis = millis();
			l->lastTick = (l->persistent ? rtcTime : millisTime);
		}
	}

	if(writeNeeded){
		write();
	}
}


void ClockMaster::addListener(ClockListener* listener){
	listener->lastTick = millis();
	listeners.push_back(listener);

	if(listener->persistent){
		auto it = persistentListeners.find(listener->ID);

		if(it == persistentListeners.end()){
			PersistentListener persistentListener;
			strcpy(persistentListener.ID, listener->ID);
			persistentListener.lastTick = listener->lastTick;
			persistentListeners[listener->ID] = persistentListener;

			write();
		}else{
			listener->lastTick = it->second.lastTick;
		}
	}
}

void ClockMaster::removeListener(ClockListener* listener){
	listeners.erase(std::remove(listeners.begin(), listeners.end(), listener), listeners.end());

	if(listener->persistent){
		persistentListeners.erase(listener->ID);
		write();
	}
}

void ClockMaster::write(){
	storage.close();

	storage = SPIFFS.open("/clock.bin", "w");
	for(auto key : persistentListeners){
		storage.write((uint8_t*)&key.second.ID, 10);
		storage.write((uint8_t*)&key.second.lastTick, 8);
	}
	storage.close();

	storage = SPIFFS.open("/clock.bin", "r");

}

void ClockMaster::read(){
	storage.seek(0);

	while(storage.available()){
		PersistentListener listener;
		size_t read = storage.read((uint8_t*)&listener.ID, 10);
		read += storage.read((uint8_t*)&listener.lastTick, 8);

		if(read < 18) return;

		persistentListeners[listener.ID] = listener;
	}
}

uint64_t ClockMaster::syncTime(){
	//TODO - add time fetching from RTC
	return millis();
}

