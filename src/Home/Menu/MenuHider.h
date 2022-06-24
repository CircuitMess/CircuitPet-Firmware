
#ifndef CIRCUITPET_FIRMWARE_MENUHIDER_H
#define CIRCUITPET_FIRMWARE_MENUHIDER_H
#include "Menu.h"


class MenuHider : LoopListener{
public:
	MenuHider(Menu* menu);
	void activity();
	void loop(uint micros) override;
private:
	Menu* menu;

	enum State{Shown, Hiding, Hidden, Showing} state = Hidden;

	const uint32_t duration = 300000; //micros
	const uint8_t deltaY = 64;
	float transition = 1.0f; //0 to a 1 real quick

	const uint32_t inactivityTimeout = 5000000; //micros
	uint32_t inactivityCount = 0;
};


#endif //CIRCUITPET_FIRMWARE_MENUHIDER_H