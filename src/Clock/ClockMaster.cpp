#include "ClockMaster.h"
#include <SPIFFS.h>
#include <Loop/LoopManager.h>
#include <CircuitPet.h>

nvs_handle ClockMaster::handle;
ClockMaster Clock;

static const char* tag = "ClockMaster";

void ClockMaster::begin(){
	auto err = nvs_open("ClockMaster", NVS_READWRITE, &handle);
	if(err != ESP_OK){
		ESP_LOGW(tag, "Clock storage initialization error: %s\n", esp_err_to_name(err));
	}

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
		lastRTCTime = newTime;
	}

	time_t rtcTime = syncTime();
	time_t millisTime = millis() / 1000;
	bool writeNeeded = false;

	for(auto l : listeners){
		uint32_t delta;
		if(l->persistent){

			//if battery power was cut, the recorded lastTick will be greater than current rtcTime, so we reset it
			if(rtcTime < l->lastTick){
				l->lastTick = rtcTime;
				l->lastTickMillis = millisTime;
				write();
				continue;
			}
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
				if(l->lastTickMillis != 0 && l->lastTickMillis >= millisTime && millisTime - l->lastTickMillis <= 0.5 * l->tickInterval){
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
	auto size = (sizeof(time_t) + 10) * persistentListeners.size();
	auto data = (uint8_t*)malloc(size);
	auto it = persistentListeners.begin();
	for(int i = 0; i < persistentListeners.size(); i++){
		memcpy(data + (sizeof(time_t) + 10) * i, (uint8_t*)&it->second.ID, 10);
		memcpy(data + (10) * i, (uint8_t*)&it->second.lastTick, sizeof(time_t));
		it++;
	}
	auto err = nvs_set_blob(handle, "ClockMaster", data, size);
	if(err != ESP_OK){
		ESP_LOGW(tag, "Clock data write failed");
		return;
	}
}

void ClockMaster::read(){

	size_t size;

	auto err = nvs_get_blob(handle, "ClockMaster", nullptr, &size);
	if(err != ESP_OK){
		ESP_LOGW(tag, "Clock data size read failed: %s", esp_err_to_name(err));
		return;
	}


	auto data = (uint8_t*)malloc(size);

	err = nvs_get_blob(handle, "ClockMaster", data, &size);
	if(err != ESP_OK){
		ESP_LOGW(tag, "Clock data read failed: %s", esp_err_to_name(err));
		return;
	}

	if(!data || !size || (size % (sizeof(time_t) + 10)) != 0){
		ESP_LOGW(tag, "Clock data read failed - data invalid");
		return;
	}

	persistentListeners.clear();

	for(int i = 0; i < size / (sizeof(time_t) + 10); ++i){
		PersistentListener listener;
		memcpy((uint8_t*)&listener.ID, data + (sizeof(time_t) + 10) * i, 10);
		memcpy((uint8_t*)&listener.lastTick, data + (10) * i, sizeof(time_t));

		persistentListeners[listener.ID] = listener;
	}
}

time_t ClockMaster::syncTime(){
	return CircuitPet.getUnixTime();
}
