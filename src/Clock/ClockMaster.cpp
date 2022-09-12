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

void ClockMaster::updatePersistent(){
	auto currentTime = CircuitPet.getUnixTime();

	bool needsWrite = false;

	for(auto listener : listeners){
		if(!listener->persistent) continue;
		auto pair = persistentListeners.find(std::string(listener->ID));
		if(pair == persistentListeners.end()) continue;

		listener->lastTick = pair->second.lastTick;

		if(currentTime < listener->lastTick){
			listener->lastTick = currentTime;
			pair->second.lastTick = currentTime;
			continue;
		}

		while(listener->lastTick + listener->tickInterval < currentTime){
			listener->lastTick += listener->tickInterval;
			listener->func();
		}

		if(pair->second.lastTick != listener->lastTick){
			pair->second.lastTick = listener->lastTick;
			needsWrite = true;
		}
	}

	if(needsWrite){
		write();
	}

	updateTime = millis();
}

void ClockMaster::loop(uint micros){
	if(millis() - updateTime < updateInterval) return;
	updateTime = millis();

	auto currentTime = CircuitPet.getUnixTime();

	bool needsWrite = false;

	for(auto listener : listeners){
		while(listener->lastTick + listener->tickInterval < currentTime){
			listener->lastTick += listener->tickInterval;
			listener->func();
		}

		if(!listener->persistent) continue;

		auto pair = persistentListeners.find(std::string(listener->ID));
		if(pair == persistentListeners.end()) continue;

		if(pair->second.lastTick != listener->lastTick){
			pair->second.lastTick = listener->lastTick;
			needsWrite = true;
		}
	}

	if(needsWrite){
		write();
	}
}


void ClockMaster::addListener(ClockListener* listener){
	listeners.push_back(listener);

	listener->lastTick = CircuitPet.getUnixTime();

	if(listener->persistent){
		auto it = persistentListeners.find(std::string(listener->ID));

		if(it == persistentListeners.end()){
			PersistentListener persistentListener;
			memcpy(persistentListener.ID, listener->ID, 10);
			persistentListener.lastTick = listener->lastTick;
			persistentListeners[std::string(listener->ID)] = persistentListener;

			write();
		}
	}
}

void ClockMaster::removeListener(ClockListener* listener){
	listeners.erase(std::remove(listeners.begin(), listeners.end(), listener), listeners.end());

	if(listener->persistent){
		persistentListeners.erase(std::string(listener->ID));
		write();
	}
}

void ClockMaster::write(){
	auto size = persistentListeners.size() * sizeof(PersistentListener);
	uint8_t data[size];
	size_t ptr = 0;
	for(auto listener : persistentListeners){
		memcpy(data + ptr, &listener.second, sizeof(PersistentListener));
		ptr += sizeof(PersistentListener);
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

	if(size == 0) return;

	uint8_t data[size];

	err = nvs_get_blob(handle, "ClockMaster", data, &size);
	if(err != ESP_OK){
		ESP_LOGW(tag, "Clock data read failed: %s", esp_err_to_name(err));
		return;
	}

	if((size % sizeof(PersistentListener)) != 0){
		ESP_LOGW(tag, "Clock data read failed - data invalid");
		return;
	}

	size_t ptr = 0;
	persistentListeners.clear();
	for(int i = 0; i < size / (sizeof(time_t) + 10); ++i){
		PersistentListener listener;
		memcpy(&listener, data + ptr, sizeof(PersistentListener));
		ptr += sizeof(PersistentListener);

		persistentListeners[listener.ID] = listener;
	}
}