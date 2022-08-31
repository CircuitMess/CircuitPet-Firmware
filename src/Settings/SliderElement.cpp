#include "SliderElement.h"

SettingsScreen::SliderElement::SliderElement(ElementContainer* partent, const char* name) : SettingsElement(partent, name){

}

void SettingsScreen::SliderElement::toggle(){
	editing = !editing;
}

void SettingsScreen::SliderElement::moveSliderValue(int8_t value){
	sliderValue = constrain(sliderValue + 20 * value, 0, 255);
}

void SettingsScreen::SliderElement::setSliderValue(uint8_t sliderValue){
	SliderElement::sliderValue = sliderValue;
}

uint8_t SettingsScreen::SliderElement::getSliderValue() const{
	return sliderValue;
}

void SettingsScreen::SliderElement::drawControl(){
	float movingCursor;
	if(sliderValue == 0){
		movingCursor = 0;
	}else{
		movingCursor = ((float)sliderValue / 255) * 51.0f;
	}
	getSprite()->drawRect(getTotalX() + 100, getTotalY() + 10, 2, 5, TFT_WHITE);
	getSprite()->drawRect(getTotalX() + 153, getTotalY() + 10, 2, 5, TFT_WHITE);
	getSprite()->drawRect(getTotalX() + 100, getTotalY() + 12, 55, 1, TFT_WHITE);
	if(editing){
		getSprite()->fillRoundRect(getTotalX() + 100 + movingCursor, getTotalY() + 8, 4, 9, 1, TFT_RED);
	}else{
		getSprite()->fillRoundRect(getTotalX() + 100 + movingCursor, getTotalY() + 9, 4, 7, 1, TFT_WHITE);
	}
}
