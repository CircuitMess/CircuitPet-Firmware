#include "MenuHider.h"
#include <Loop/LoopManager.h>

MenuHider::MenuHider(Menu* menu): menu(menu){
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

void MenuHider::hide(){
	switch (state){
		case Hidden:
			break;
		case Shown:
			state = Hiding;
			LoopManager::addListener(this);
			break;
		case Hiding:
			break;
		case Showing:
			state = Hiding;
			break;
	}
}

void MenuHider::loop(uint deltaMicros){
	auto easeOutElastic = [](float t){
		constexpr float c4 = (2.0f * M_PI) / 3.0f;
		return t <= 0 ? 0 : t >= 1 ? 1 : pow(2.0f, -10.0f * t) * sin((t * 10.0f - 0.75f) * c4) + 1.0f;
	};

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
			menu->setOffsetY(deltaY*(1.0f - easeOutElastic(1.0f - transition)));
			if(transition <= 0.0f){
				transition = 0.0f;
				menu->setOffsetY(0);
				inactivityCount = 0;
				state = Shown;
			}
			break;

		case Hiding:
			transition += (float)deltaMicros/(float)duration;
			menu->setOffsetY(deltaY*pow(transition, 5));
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
