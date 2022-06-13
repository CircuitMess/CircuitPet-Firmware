
#ifndef CIRCUITPET_FIRMWARE_MENUHIDER_H
#define CIRCUITPET_FIRMWARE_MENUHIDER_H
#include "Menu.h"


class MenuHider {
public:
	MenuHider(Menu menu);
	void activity();
private:
	Menu menu;
	bool active = true;
	const uint32_t inactivityTimeout = 10000; //ms
};


#endif //CIRCUITPET_FIRMWARE_MENUHIDER_H
