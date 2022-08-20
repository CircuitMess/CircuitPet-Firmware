#ifndef WHEELSON_FIRMWARE_TEXTELEMENT_H
#define WHEELSON_FIRMWARE_TEXTELEMENT_H

#include "SettingsElement.h"

namespace SettingsScreen {
class TextElement : public SettingsElement {
public:
	TextElement(ElementContainer* partent, String name);

	void toggle() override;

private:

protected:
	void drawControl() override;
};
}

#endif //WHEELSON_FIRMWARE_TEXTELEMENT_H
