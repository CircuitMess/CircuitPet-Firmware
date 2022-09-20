#include "ClockMaster.h"
#include <SPIFFS.h>
#include <Loop/LoopManager.h>
#include <CircuitPet.h>

nvs_handle ClockMaster::handle;
ClockMaster Clock;

ClockMaster::ClockMaster(){
	times.reserve(updateCount * 2);
	listeners.reserve(6);
}

static const char* tag = "ClockMaster";

void ClockMaster::begin(){
	auto err = nvs_open("ClockMaster", NVS_READWRITE, &handle);
	if(err != ESP_OK){
		ESP_LOGW(tag, "Clock storage initialization error: %s\n", esp_err_to_name(err));
	}

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

	times.push_back(CircuitPet.getUnixTime());
	if(times.size() < updateCount) return;

	processTime();
}

void ClockMaster::processTime(){
	while(times.size() > updateCount){
		times.erase(times.begin());
	}

	while(times.size() < updateCount){
		times.push_back(CircuitPet.getUnixTime());
	}

	for(int k = 0; k < updateCount/2; k++){
		for(int i = 0; i < updateCount; i++){
			for(int j = 0; j < updateCount; j++){
				if(i == j) continue;
				if(abs(difftime(times[i], times[j])) <= 5.0f){
					times[i] = times[j];
				}
			}
		}
	}

	uint8_t votes[updateCount] = {0};
	for(int i = 0; i < updateCount; ++i){
		for(int j = 0; j < updateCount; ++j){
			if(i == j) continue;
			if(abs(difftime(times[i], times[j])) <= 2.0f){
				votes[i]++;
			}
		}
	}

	size_t chosen = *std::max_element(votes, votes+updateCount);
	auto currentTime = times[chosen];
	times.clear();

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

	nvs_commit(handle);
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