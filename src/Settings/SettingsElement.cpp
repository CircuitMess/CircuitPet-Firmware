#include "SettingsElement.h"

SettingsScreen::SettingsElement::SettingsElement(ElementContainer* parent, const char* name) : CustomElement(parent, 160, 18), name(name){

}

void SettingsScreen::SettingsElement::setIsSelected(bool isSelected){
	SettingsElement::selected = isSelected;

}

void SettingsScreen::SettingsElement::draw(){
	getSprite()->setTextColor(TFT_WHITE);
	getSprite()->setTextSize(1);
	getSprite()->setTextFont(1);
	getSprite()->setCursor(getTotalX() + 5, getTotalY() + 7);
	getSprite()->println(name);
	if(selected){
		getSprite()->drawRect(getTotalX() + 3, getTotalY() + 3, 86, 15, TFT_WHITE);
	}
	drawControl();
}

bool SettingsScreen::SettingsElement::isSelected() const{
	return selected;
}
