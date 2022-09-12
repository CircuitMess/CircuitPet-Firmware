#include <Loop/LoopManager.h>
#include "AutoShutdown.h"
#include "Clock/ClockMaster.h"
#include <Input/Input.h>
#include <Settings.h>
#include <CircuitPet.h>

const uint16_t AutoShutdown::sleepTimes[] = { 0, 15, 30, 60, 3 * 60 };

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

	auto delay = sleepTimes[index] * 1000;
	if(millis() - activity >= delay){
		CircuitPet.sleep();
		activity = millis();
		LoopManager::resetTime();
		Clock.loop(0);
	}
}
