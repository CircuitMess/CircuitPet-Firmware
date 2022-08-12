#ifndef CIRCUITPET_FIRMWARE_SETTINGSSCREEN_H
#define CIRCUITPET_FIRMWARE_SETTINGSSCREEN_H

#include <UI/LinearLayout.h>
#include "DiscreteSlider.h"
#include "BooleanElement.h"
#include <Input/InputListener.h>
#include "TextElement.h"
#include "SliderElement.h"
#include <FS.h>
#include "../State.h"
#include <Loop/LoopListener.h>
#include <UI/Screen.h>
#include <list>

namespace SettingsScreen {
class SettingsScreen : public State, private InputListener, private LoopListener {
public:

	SettingsScreen(Display& display);

	virtual ~SettingsScreen();

	void onStart() override;

	void onStop() override;

	void draw();

	void loop(uint micros) override;

private:
	static SettingsScreen* instance;

	Screen screen;

	LinearLayout* screenLayout;
	DiscreteSlider* shutDownSlider;
	SliderElement* brightnessSlider;
	BooleanElement* soundSwitch;
	SliderElement* rgbSlider;
	TextElement* inputTest;
	TextElement* save;

	std::array<SettingsElement*, 6> elements;

	bool editMode = false;
	uint8_t scrollStep = 1;
	uint8_t scrollCount = 0;
	int selectedSetting = 0;

	void buildUI();
	void buttonPressed(uint id) override;
	void buttonReleased(uint id) override;

	Color* backgroundBuffer = nullptr;
	bool cycleRGB = false;
};
}

#endif //CIRCUITPET_FIRMWARE_SETTINGSSCREEN_H
