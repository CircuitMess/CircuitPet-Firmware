#include "StatsManager.h"
#include <SPIFFS.h>
#include "../Clock/ClockMaster.h"

StatsManager StatMan;
static const char* tag = "StatsManager";

const uint16_t StatsManager::levelupThresholds[] = { 100, 200, 300, 400, 500 }; //TODO - settati levelUp threshove
const Stats StatsManager::hourlyDecrement = { 2, 5, 0 };

StatsManager::StatsManager() : timedUpdateListener(100, false, true, "StatsMan", [this](){ timedUpdate(); }){
}

void StatsManager::begin(){
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

void StatsManager::setPaused(bool pause){
	paused = pause;
}

void StatsManager::store(){
	File f = SPIFFS.open("/stats.bin", "w");
	f.write((uint8_t*)&stats, sizeof(Stats));
	f.write(gameOverCount);
	f.write(hatched);
	f.close();
}

void StatsManager::load(){
	File f = SPIFFS.open("/stats.bin", "r");

	if(!f || f.available() != sizeof(Stats) + sizeof(gameOverCount) + sizeof(hatched)){
		ESP_LOGW(tag, "Stats file not found or corrupt! Setting defaults.");
		stats.happiness = 100;
		stats.oilLevel = 100;
		stats.experience = 0;
		gameOverCount = 0;
		hatched = false;
		return;
	}

	f.read((uint8_t*)&stats, sizeof(Stats));

	if(stats.happiness > 100 || stats.oilLevel > 100){
		ESP_LOGW(tag, "Stats file not found or corrupt! Setting defaults.");
		stats.happiness = 100;
		stats.oilLevel = 100;
		stats.experience = 0;
		hatched = false;
		return;
	}
	gameOverCount = f.read();
	hatched = f.read();

	f.close();
}

void StatsManager::timedUpdate(){
	if(paused) return;

	stats -= hourlyDecrement;

	if(stats.happiness == 0){
		gameOverCount++;
	}else{
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
