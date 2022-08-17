#ifndef CIRCUITPET_FIRMWARE_BOOLEANELEMENT_H
#define CIRCUITPET_FIRMWARE_BOOLEANELEMENT_H

#include "SettingsElement.h"

namespace SettingsScreen {
class BooleanElement : public SettingsElement {
public:
	BooleanElement(ElementContainer* parent, String name);

	void toggle() override;

	bool getBooleanSwitch() const;

	void setBooleanSwitch(bool booleanSwitch);

private:

	bool booleanSwitch = false;

protected:
	void drawControl() override;
};
}

#endif //CIRCUITPET_FIRMWARE_BOOLEANELEMENT_H
