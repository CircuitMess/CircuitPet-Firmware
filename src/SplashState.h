
#ifndef CIRCUITPET_FIRMWARE_SPLASHSTATE_H
#define CIRCUITPET_FIRMWARE_SPLASHSTATE_H


#include <Input/InputListener.h>
#include <Loop/LoopListener.h>
#include <Display/Sprite.h>
#include "State.h"
#include "Home/Menu/Menu.h"
#include "GameEngine/Game.h"

class SplashState : public State, private LoopListener, private InputListener {
public:
	SplashState(Sprite* base, const MenuItemData& menuItem);
	void loop(uint micros) override;

protected:
	void onStart() override;
	void onStop() override;

private:
	Sprite* base;
	const uint minSplashTime = 2000;
	uint startTime;
	bool splashDone = false;
	Game* game;
	bool startedLoading = false;
	const char* instructPath;
	const char* splashPath;

	void buttonPressed(uint i) override;
};


#endif //CIRCUITPET_FIRMWARE_SPLASHSTATE_H
