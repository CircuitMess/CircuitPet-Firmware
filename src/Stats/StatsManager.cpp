#include "StatsManager.h"
#include <SPIFFS.h>
#include "../Clock/ClockMaster.h"

StatsManager StatMan;
static const char* tag = "StatsManager";
nvs_handle StatsManager::handle;

const uint16_t StatsManager::levelupThresholds[] = { 50, 200, 1000, 4000, 10000 };
const Stats StatsManager::hourlyDecrement = { 5, 3, 0 };

StatsManager::StatsManager() : timedUpdateListener(3600, true, "StatsMan", [this](){ timedUpdate(); }){
}

void StatsManager::begin(){
	auto err = nvs_open("StatsMan", NVS_READWRITE, &handle);
	if(err != ESP_OK){
		ESP_LOGW(tag, "Clock storage initialization error: %s\n", esp_err_to_name(err));
	}

	reserve(6);

	load();

	Clock.addListener(&timedUpdateListener);
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
	if(!hatched) return;

	uint8_t oldLevel = getLevel();
	stats += delta;

	if(stats.oilLevel > 0){
		gameOverCount = 0;
	}

	ESP_LOGI(tag, "%d, %d, %d\n", stats.happiness, stats.experience, stats.oilLevel);

	bool levelUp = (oldLevel != getLevel());

	iterateListeners([this, &levelUp](StatsChangedListener* listener){
		listener->statsChanged(stats, levelUp);
	});

	store();
}

bool StatsManager::hasDied() const{
	return gameOverCount > gameOverHours; //dies after 24hrs of zero happiness
}


const Stats& StatsManager::get() const{
	return stats;
}

uint8_t StatsManager::getLevel() const{
	return getLevel(stats.experience);
}

uint8_t StatsManager::getLevel(uint16_t exp) const{
	const uint8_t levelupsNum = sizeof(levelupThresholds) / sizeof(uint16_t);
	uint16_t requiredXP = 0;
	for(uint8_t i = 0; i < levelupsNum; i++){
		requiredXP += levelupThresholds[i];
		if(exp < requiredXP){
			return i + 1;
		}
	}
	return levelupsNum + 1;
}

uint8_t StatsManager::getExpPercentage() const{
	return getExpPercentage(stats.experience);
}


uint8_t StatsManager::getExpPercentage(uint16_t exp) const{
	if(getLevel(exp) == 1){
		return (uint8_t)(((float)exp*100)/levelupThresholds[0]);
	}

	if(getLevel(exp) == 6){
		return 100;
	}


	uint16_t prevThreshold = 0;
	for(uint8_t i = 0; i < getLevel(exp) - 1; i++){
		prevThreshold += levelupThresholds[i];
	}

	return  (uint8_t)((float)(exp-prevThreshold)* 100.0/(levelupThresholds[getLevel(exp)-1]));
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

	nvs_commit(handle);
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
	if(hasDied() || !hatched) return;

	stats -= hourlyDecrement;

	if(stats.oilLevel == 0 && gameOverCount <= gameOverHours){
		gameOverCount++;
	}else if(stats.oilLevel > 0){
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
