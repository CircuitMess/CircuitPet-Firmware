#ifndef CIRCUITPET_FIRMWARE_DEATHSTATE_H
#define CIRCUITPET_FIRMWARE_DEATHSTATE_H

#include <Input/InputListener.h>
#include "State.h"
#include <Display/GIFAnimatedSprite.h>
#include <Display/AnimatedSprite.h>
#include "Home/Sprites/BgSprite.h"
#include <Display/AnimatedSprite.h>


class DeathState : public State, private LoopListener{
public:
	DeathState(Sprite* base);

	void loop(uint micros) override;

protected:
	void onStart() override;
	void onStop() override;

private:

	static DeathState* instance;

	AnimatedSprite explosion;
	GIFAnimatedSprite duck;
	Sprite* base;
	BgSprite bg;

	bool exit = false;
	uint32_t duckRemoveMicros = 0;
	static constexpr uint32_t duckVisibleMicros = 560000;
	static constexpr uint32_t explosionStart = 1000000;
	uint32_t explosionMicros = 0;
	bool exploded = false;
	bool duckVisible = true;
	uint32_t exitMicros = 0;
};

#endif //CIRCUITPET_FIRMWARE_DEATHSTATE_H
