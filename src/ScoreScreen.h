
#ifndef CIRCUITPET_FIRMWARE_SCORESCREEN_H
#define CIRCUITPET_FIRMWARE_SCORESCREEN_H

#include <Input/InputListener.h>
#include <Loop/LoopListener.h>
#include "State.h"
#include "Stats/Stats.hpp"
#include "Display/Sprite.h"
#include "Home/Sprites/StatSprite.h"

class ScoreScreen : public State, private InputListener, private LoopListener{
public:
	ScoreScreen(Stats stats);

protected:
	void onStart() override;
	void onStop() override;

private:
	void loop(uint micros) override;

	void exit();

	const Stats stats;
	Sprite* base;
	File frameFile;

	StatSprite oil;
	StatSprite happiness;
	StatSprite xp;

	float easeTimer = 0;
	Stats currentStats;
	Stats targetStats;
	Stats prevStats;
	static constexpr float easeTime = 1.0f;
	static constexpr uint32_t exitTime = 4000000;

	bool easeDone = false;
	uint32_t t = 0;

	void buttonPressed(uint i) override;
};


#endif //CIRCUITPET_FIRMWARE_SCORESCREEN_H
