#ifndef CIRCUITPET_FIRMWARE_HATCHINGSTATE_H
#define CIRCUITPET_FIRMWARE_HATCHINGSTATE_H

#include <Input/InputListener.h>
#include "State.h"
#include <Display/GIFAnimatedSprite.h>

class HatchingState : public State, private LoopListener, private InputListener {
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
	bool accepted = false;
	uint32_t blinkTime = 0;

	void buttonPressed(uint i) override;
};


#endif //CIRCUITPET_FIRMWARE_HATCHINGSTATE_H
