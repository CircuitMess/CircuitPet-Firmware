#ifndef CIRCUITPET_FIRMWARE_HATCHINGSTATE_H
#define CIRCUITPET_FIRMWARE_HATCHINGSTATE_H


#include "State.h"
#include <Display/GIFAnimatedSprite.h>

class HatchingState : public State, private LoopListener {
public:
	HatchingState(Sprite* base);

	void loop(uint micros) override;

protected:
	void onStart() override;
	void onStop() override;


private:
	GIFAnimatedSprite gif;
	Sprite* base;

	bool exit = false;
};


#endif //CIRCUITPET_FIRMWARE_HATCHINGSTATE_H
