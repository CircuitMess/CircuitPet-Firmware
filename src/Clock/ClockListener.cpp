#include "ClockListener.h"

ClockListener::ClockListener(time_t tickInterval, bool single, bool persistent, const char* id, std::function<void()> func)
		: tickInterval(tickInterval), single(single), persistent(persistent), func(func){
	strncpy(ID, id, 10);
}

void ClockListener::reset(){

}
