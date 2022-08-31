#ifndef CIRCUITPET_FIRMWARE_HATCHINGSTATE_H
#define CIRCUITPET_FIRMWARE_HATCHINGSTATE_H

#include <Input/InputListener.h>
#include "State.h"
#include <Display/AnimatedSprite.h>

class HatchingState : public State, private LoopListener, private InputListener {
public:
	HatchingState(Sprite* base);

	void loop(uint micros) override;

protected:
	void onStart() override;
	void onStop() override;


private:
	AnimatedSprite gif;
	Sprite* base;

	static HatchingState* instance;

	bool exit = false;
	bool accepted = false;
	uint32_t blinkTime = 0;

	void buttonPressed(uint i) override;
};


#endif //CIRCUITPET_FIRMWARE_HATCHINGSTATE_H
