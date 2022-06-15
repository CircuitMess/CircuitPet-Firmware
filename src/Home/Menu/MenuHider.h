
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

	bool active = false;
	bool transition = false;
	const uint32_t duration = 300000; //micros
	uint32_t durationCount = 0;
	const uint8_t deltaY = 64;
	unsigned long startTime;

	const uint32_t inactivityTimeout = 30000000; //micros
	uint32_t inactivityCount = 0;
};


#endif //CIRCUITPET_FIRMWARE_MENUHIDER_H
