#ifndef CIRCUITPET_FIRMWARE_USERHWTEST_H
#define CIRCUITPET_FIRMWARE_USERHWTEST_H

#include <Arduino.h>
#include <Input/InputListener.h>
#include <unordered_map>
#include <Loop/LoopListener.h>
#include "State.h"
#include <Display/Sprite.h>

class UserHWTest : public State, private InputListener, public LoopListener {
public:
	UserHWTest(Sprite* base, std::function<void()> doneFunc);

	void onStart() override;
	void onStop() override;
	void loop(uint micros) override;

private:
	Sprite* base;
	Sprite bgSprite;

	std::array<bool, 16> buttons;
	uint8_t buttonCount = 0;
	struct KeyCoord { uint8_t x, y; };
	static const std::unordered_map<uint8_t, KeyCoord> KeyCoords;

	uint32_t noteTime;
	uint8_t noteIndex;
	struct Note { uint16_t freq, duration; };
	static const std::vector<Note> Notes;

	uint8_t stage = 0;
	std::function<void()> doneFunc;

	void buttonPressed(uint i) override;
	void startBuzz();

};

#endif //CIRCUITPET_FIRMWARE_USERHWTEST_H
