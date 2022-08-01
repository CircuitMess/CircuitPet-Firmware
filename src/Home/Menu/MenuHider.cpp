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
	switch(state){
		case Shown:
			inactivityCount += deltaMicros;
			if(inactivityCount >= inactivityTimeout){
				state = Hiding;
			}
			break;

		case Hidden:
			break;

		case Showing:
			transition -= (float)deltaMicros/(float)duration;
			menu->setOffsetY(deltaY*transition);
			if(transition <= 0.0f){
				transition = 0.0f;
				menu->setOffsetY(0);
				inactivityCount = 0;
				state = Shown;
			}
			break;

		case Hiding:
			transition += (float)deltaMicros/(float)duration;
			menu->setOffsetY(deltaY*transition);
			if(transition >= 1.0f){
				transition = 1.0f;
				menu->setOffsetY(deltaY);
				state = Hidden;
				LoopManager::removeListener(this);
			}
			break;
	}
}

MenuHider::State MenuHider::getState() const{
	return state;
}
