#include "StatsManager.h"
#include <SPIFFS.h>

StatsManager StatMan;
static const char* tag = "StatsManager";

const uint16_t StatsManager::levelupThresholds[] = { 100, 200, 300, 400, 500 }; //TODO - settati levelUp threshove
const Stats StatsManager::hourlyDecrement = { 2, 5, 0 };

StatsManager::StatsManager() : timedUpdateListener(3600000, false, true, "StatsMan", [this](){ timedUpdate(); }){
}

void StatsManager::begin(){

}

void StatsManager::update(Stats delta){
	uint8_t oldLevel = getLevel();
	stats -= delta;
	ESP_LOGI(tag, "%d, %d, %d\n", stats.happiness, stats.experience, stats.oilLevel);

	bool levelUp = (oldLevel != getLevel());

	iterateListeners([this, &levelUp](StatsChangedListener* listener){
		listener->statsChanged(stats, levelUp);
	});
}

bool StatsManager::hasDied(){
	return gameOverCount > 24; //dies after 24hrs of zero happiness
}


const Stats& StatsManager::get(){
	return stats;
}

uint8_t StatsManager::getLevel(){
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
	File f = SPIFFS.open("/stats.bin", "width");
	f.write((uint8_t*)&stats, sizeof(Stats));
	f.write(gameOverCount);
	f.close();
}

void StatsManager::load(){
	File f = SPIFFS.open("/stats.bin", "r");

	if(!f || f.available() != sizeof(Stats) + sizeof(gameOverCount)){
		ESP_LOGW(tag, "Stats file not found or corrupt! Setting defaults.");
		stats.happiness = 100;
		stats.oilLevel = 100;
		stats.experience = 0;
		return;
	}

	f.read((uint8_t*)&stats, sizeof(Stats));
	gameOverCount = f.read();
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

}
