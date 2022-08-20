#ifndef CIRCUITPET_FIRMWARE_USERHWTEST_H
#define CIRCUITPET_FIRMWARE_USERHWTEST_H

#include <Arduino.h>
#include <Input/InputListener.h>
#include <unordered_map>
#include <Loop/LoopListener.h>
#include "State.h"
#include "GameEngine/PixelDim.hpp"
#include <Display/Sprite.h>

class UserHWTest : public State, private InputListener, public LoopListener {
public:
	UserHWTest(Sprite* base, std::function<void()> doneFunc = {});

	void onStart() override;
	void onStop() override;
	void loop(uint micros) override;

private:
	Sprite* base;
	fs::File bg;
	fs::File device;

	std::array<bool, 4> inputTested;
	static const std::unordered_map<uint8_t, uint8_t> KeyMap;
	static const std::array<PixelDim, 4> KeyPos;

	uint32_t noteTime;
	uint8_t noteIndex;
	struct Note { uint16_t freq, duration; };
	static const std::vector<Note> Notes;

	uint8_t stage = 0;
	std::function<void()> doneFunc;

	uint8_t colorIndex = 0;
	uint32_t colorTime = 0;

	void buttonPressed(uint i) override;
	void stage2();
	void stage3();

	void draw();
};

#endif //CIRCUITPET_FIRMWARE_USERHWTEST_H
