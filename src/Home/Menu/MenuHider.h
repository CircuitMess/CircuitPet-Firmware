
#ifndef CIRCUITPET_FIRMWARE_MENUHIDER_H
#define CIRCUITPET_FIRMWARE_MENUHIDER_H
#include "Menu.h"


class MenuHider : private LoopListener{
public:
	enum State{Shown, Hiding, Hidden, Showing};

	MenuHider(Menu* menu);
	void activity();
	void hide();
	void loop(uint micros) override;
	State getState() const;

	bool allowAction();

private:
	Menu* menu;

	State state = Hidden;

	const uint32_t duration = 1400000; //micros
	const int16_t deltaY = 47;
	float transition = 1.0f; //0 to a 1 real quick

	const uint32_t inactivityTimeout = 5000000; //micros
	uint32_t inactivityCount = 0;
};


#endif //CIRCUITPET_FIRMWARE_MENUHIDER_H
