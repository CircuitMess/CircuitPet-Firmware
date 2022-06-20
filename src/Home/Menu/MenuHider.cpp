#include "MenuHider.h"
#include <Loop/LoopManager.h>

MenuHider::MenuHider(Menu* menu): menu(menu){
	activity(); // used to set inactivityCount
}

void MenuHider::activity(){
	switch (state){
		case Hidden:
			LoopManager::addListener(this);
			state = Showing;
		case Shown:
			inactivityCount = 0;
			break;
		case Hiding:
			state = Showing;
			break;
		case Showing:
			break;
	}
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
