#include "ClockListener.h"

ClockListener::ClockListener(time_t tickInterval, bool persistent, const char* id, std::function<void()> func)
		: tickInterval(tickInterval), persistent(persistent), func(func){
	strncpy(ID, id, 10);
}

void ClockListener::reset(){

}
