
#ifndef CIRCUITPET_FIRMWARE_SCORESCREEN_H
#define CIRCUITPET_FIRMWARE_SCORESCREEN_H

#include <Input/InputListener.h>
#include <Loop/LoopListener.h>
#include "State.h"
#include "Stats/Stats.hpp"
#include "Display/Sprite.h"

class ScoreScreen : public State, private InputListener{
public:
	ScoreScreen(Stats stats);

protected:
	void onStart() override;
	void onStop() override;

private:
	Stats stats;
	Sprite* base;
	bool exit = false;

	void buttonPressed(uint i) override;
};


#endif //CIRCUITPET_FIRMWARE_SCORESCREEN_H
