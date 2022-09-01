#include "StatsManager.h"
#include <SPIFFS.h>
#include "../Clock/ClockMaster.h"

StatsManager StatMan;
static const char* tag = "StatsManager";
nvs_handle StatsManager::handle;

const uint16_t StatsManager::levelupThresholds[] = { 50, 150, 350, 750, 1550 }; //TODO - settati levelUp threshove
const Stats StatsManager::hourlyDecrement = { 2, 5, 0 };

StatsManager::StatsManager() : timedUpdateListener(3600, false, true, "StatsMan", [this](){ timedUpdate(); }){
}

void StatsManager::begin(){
	auto err = nvs_open("StatsMan", NVS_READWRITE, &handle);
	if(err != ESP_OK){
		ESP_LOGW(tag, "Clock storage initialization error: %s\n", esp_err_to_name(err));
	}

	load();
}

void StatsManager::reset(){
	stats.happiness = 100;
	stats.oilLevel = 100;
	stats.experience = 0;
	gameOverCount = 0;
	hatched = false;
	store();
}

void StatsManager::update(Stats delta){
	uint8_t oldLevel = getLevel();
	stats += delta;
	ESP_LOGI(tag, "%d, %d, %d\n", stats.happiness, stats.experience, stats.oilLevel);

	bool levelUp = (oldLevel != getLevel());

	iterateListeners([this, &levelUp](StatsChangedListener* listener){
		listener->statsChanged(stats, levelUp);
	});

	store();
}

bool StatsManager::hasDied() const{
	return gameOverCount > 24; //dies after 24hrs of zero happiness
}


const Stats& StatsManager::get() const{
	return stats;
}

uint8_t StatsManager::getLevel() const{
	const uint8_t levelupsNum = sizeof(levelupThresholds) / sizeof(uint16_t);
	for(uint8_t i = 0; i < levelupsNum; i++){
		if(stats.experience < levelupThresholds[i]){
			return i + 1;
		}
	}
	return levelupsNum + 1;
}

uint8_t StatsManager::getExpPercentage() const{
	if(getLevel() == 1) return (uint8_t)stats.experience/levelupThresholds[0];
	uint8_t prevThreshold = levelupThresholds[getLevel()-2];
	return  (uint8_t)((stats.experience-prevThreshold)* 100/(levelupThresholds[getLevel()-1]-prevThreshold)) ;
}

void StatsManager::setPaused(bool pause){
	pause ? Clock.removeListener(&timedUpdateListener) : Clock.addListener(&timedUpdateListener);
}

void StatsManager::store(){
	auto size = sizeof(Stats) + sizeof(gameOverCount) + sizeof(hatched);
	uint8_t data[size];

	memcpy(data, (uint8_t*)&stats, sizeof(Stats));
	memcpy(data + sizeof(Stats), &gameOverCount, 1);
	memcpy(data + sizeof(Stats) + sizeof(gameOverCount), &hatched, 1);


	auto err = nvs_set_blob(handle, "StatsMan", data, size);
	if(err != ESP_OK){
		ESP_LOGW(tag, "Clock data write failed");
		return;
	}
}

void StatsManager::load(){
	auto size = sizeof(Stats) + sizeof(gameOverCount) + sizeof(hatched);
	uint8_t data[size];

	auto err = nvs_get_blob(handle, "StatsMan", data, &size);
	if(err != ESP_OK){
		ESP_LOGW(tag, "Clock data read failed: %s", esp_err_to_name(err));
		ESP_LOGW(tag, "Stats data not found or corrupt! Setting defaults.");
		stats.happiness = 100;
		stats.oilLevel = 100;
		stats.experience = 0;
		gameOverCount = 0;
		hatched = false;
		return;
	}

	if(!data || !size || size != sizeof(Stats) + sizeof(gameOverCount) + sizeof(hatched)){
		ESP_LOGW(tag, "Clock data read failed - data invalid");
		ESP_LOGW(tag, "Stats data not found or corrupt! Setting defaults.");
		stats.happiness = 100;
		stats.oilLevel = 100;
		stats.experience = 0;
		gameOverCount = 0;
		hatched = false;
		return;
	}
	memcpy(&stats, data, sizeof(Stats));

	if(stats.happiness > 100 || stats.oilLevel > 100){

		ESP_LOGW(tag, "Stats file not found or corrupt! Setting defaults.");
		stats.happiness = 100;
		stats.oilLevel = 100;
		stats.experience = 0;
		gameOverCount = 0;
		hatched = false;
		return;
	}
	memcpy(&gameOverCount, data + sizeof(Stats), 1);
	memcpy(&hatched, data + sizeof(Stats) + sizeof(gameOverCount), 1);
}

void StatsManager::timedUpdate(){
	stats -= hourlyDecrement;

	if(stats.happiness == 0 && gameOverCount <= 24){
		gameOverCount++;
	}else if(stats.happiness > 0){
		gameOverCount = 0;
	}

	iterateListeners([this](StatsChangedListener* listener){
		listener->statsChanged(stats, false);
	});

	store();
}

bool StatsManager::isHatched() const{
	return hatched;
}

void StatsManager::setHatched(bool hatched){
	StatsManager::hatched = hatched;
	store();
}
