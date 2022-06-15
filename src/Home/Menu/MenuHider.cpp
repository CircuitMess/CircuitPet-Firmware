#include "MenuHider.h"
#include <Loop/LoopManager.h>

MenuHider::MenuHider(Menu* menu): menu(menu){
	activity(); // used to set inactivityCount
}

void MenuHider::activity(){
	if(active) return;
	LoopManager::addListener(this);
	active = true;
	inactivityCount = 0;
	if(transition) return;
	startTime = micros();
	transition = true;
}

void MenuHider::loop(uint deltaMicros){
	if(!transition){
		inactivityCount += deltaMicros;
		if(inactivityCount >= inactivityTimeout){
			active = false;
			transition = true;
			durationCount = 0;
			startTime = micros();
		}
		return;
	}
	durationCount += deltaMicros;
	float t =  (float)(micros() - startTime) /duration;
	if(active){
		menu->setOffsetY(deltaY*(1.0f-t));
	}else{
		menu->setOffsetY(deltaY*t);
	}

	if(durationCount >= duration){
		transition = false;
		if(active){
			menu->setOffsetY(0);
		}else{
			menu->setOffsetY(deltaY);
			LoopManager::removeListener(this);
		}
		durationCount = 0;

	}
}
