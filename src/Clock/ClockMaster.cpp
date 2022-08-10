#include "ClockMaster.h"
#include <SPIFFS.h>
#include <Loop/LoopManager.h>
#include <CircuitPet.h>

ClockMaster Clock;

static const char* tag = "ClockMaster";

void ClockMaster::begin(){
	lastRTCTime = syncTime();
	read();

	LoopManager::addListener(this);
}

void ClockMaster::loop(uint micros){
	syncTimeMicros += micros;

	if(syncTimeMicros >= syncTimeInterval){
		syncTimeMicros = 0;
		time_t newTime = syncTime();

		int delta = newTime - lastRTCTime - syncTimeInterval / 1000000;

		if(abs(delta) > 0){
			ESP_LOGI(tag, "RTC - millis difference over last 60 seconds: %d\n", delta);
		}
	}

	time_t rtcTime = syncTime();
	time_t millisTime = millis() / 1000;
	bool writeNeeded = false;

	for(auto l : listeners){
		uint32_t delta;
		if(l->persistent){
			if(rtcTime < l->lastTick) continue;
			delta = rtcTime - l->lastTick;
		}else{
			delta = millisTime - l->lastTick;
		}


		if(delta >= l->tickInterval){
			if(l->lastTick == 0){
				//in case of first listener tick, delta will be much higher since lastTick == 0
				l->func();
			}else{
				for(int i = 0; i < (delta / l->tickInterval); i++){
					l->func();
				}
			}

			if(l->persistent){
				if(l->lastTickMillis != 0 && millisTime - l->lastTickMillis <= 0.5 * l->tickInterval){
					//ignore tick because of discrepancy between RTC and internal clock, probably because of short tick interval and RTC syncing edge cases
					ESP_LOGW(tag, "RTC - millis difference too great, %s tick ignored\n", l->ID);
					continue;
				}
				persistentListeners[l->ID].lastTick = l->lastTick = rtcTime;
				writeNeeded = true;
			}else{
				l->lastTick = millisTime;
			}

			l->lastTickMillis = millisTime;
		}
	}

	if(writeNeeded){
		write();
	}
}


void ClockMaster::addListener(ClockListener* listener){
	if(!listener->persistent){
		listener->lastTick = millis();
	}
	listener->lastTickMillis = millis();
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
	storage = SPIFFS.open("/clock.bin", "w");
	storage.seek(0);
	for(auto key : persistentListeners){
		storage.write((uint8_t*)&key.second.ID, 10);
		storage.write((uint8_t*)&key.second.lastTick, sizeof(time_t));
	}
	storage.close();
}

void ClockMaster::read(){
	storage = SPIFFS.open("/clock.bin", "r");

	while(storage.available()){
		PersistentListener listener;
		size_t read = storage.read((uint8_t*)&listener.ID, 10);
		read += storage.read((uint8_t*)&listener.lastTick, sizeof(time_t));

		if(read < 10 + sizeof(time_t)) return;

		persistentListeners[listener.ID] = listener;
	}
	storage.close();
}

time_t ClockMaster::syncTime(){
	return CircuitPet.getUnixTime();
}

