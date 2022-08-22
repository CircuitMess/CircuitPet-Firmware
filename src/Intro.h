#ifndef CIRCUITPET_FIRMWARE_INTRO_H
#define CIRCUITPET_FIRMWARE_INTRO_H

#include "State.h"
#include <Display/GIFAnimatedSprite.h>
#include <Display/AnimatedSprite.h>
class Intro : public State, private LoopListener {
public:
	Intro(Sprite* base);

	void loop(uint micros) override;

protected:
	void onStart() override;
	void onStop() override;


private:
	static Intro* instance;

	AnimatedSprite gif;
	Sprite* base;

	bool exit = false;
	bool firstLoop = true;
};


#endif //CIRCUITPET_FIRMWARE_INTRO_H
