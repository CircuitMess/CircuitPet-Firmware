#include <Loop/LoopManager.h>
#include "AutoShutdown.h"
#include <Input/Input.h>
#include <Settings.h>
#include <CircuitPet.h>

const uint8_t AutoShutdown::minutes[] = { 0, 1, 5, 15, 30 };

void AutoShutdown::begin(){
	LoopManager::addListener(this);
	Input::getInstance()->addListener(this);

	activity = millis();
}

void AutoShutdown::buttonPressed(uint i){
	activity = millis();
}

void AutoShutdown::loop(uint micros){
	auto index = Settings.get().shutdownTime;
	if(index < 0 || index >= 5) index = 0;

	if(index == 0) return;

	auto delay = minutes[index] * 60 * 1000;
	if(millis() - activity >= delay){
		CircuitPet.fadeOut();
		CircuitPet.shutdown();
	}
}
