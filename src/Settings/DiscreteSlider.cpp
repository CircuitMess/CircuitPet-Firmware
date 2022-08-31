#include "DiscreteSlider.h"


SettingsScreen::DiscreteSlider::DiscreteSlider(ElementContainer* parent, const char* name, std::vector <uint8_t> shutDownTime) : SettingsElement(parent, name),
																																 shutDownTime(shutDownTime){

}

void SettingsScreen::DiscreteSlider::toggle(){
	editing = !editing;
}

void SettingsScreen::DiscreteSlider::selectNext(){
	if(shutDownTime.empty()) return;
	index += 1;
	index = min(index, (int)shutDownTime.size() - 1);
}

void SettingsScreen::DiscreteSlider::selectPrev(){
	if(shutDownTime.empty()) return;
	index -= 1;
	index = max(index, 0);

}

void SettingsScreen::DiscreteSlider::drawControl(){
	long movingCursor;

	movingCursor = map(index, 0, shutDownTime.size() - 1, 0, 51);

	getSprite()->drawRect(getTotalX() + 100, getTotalY() + 12, 2, 5, TFT_WHITE);
	getSprite()->drawRect(getTotalX() + 153, getTotalY() + 12, 2, 5, TFT_WHITE);
	getSprite()->drawRect(getTotalX() + 100, getTotalY() + 14, 55, 1, TFT_WHITE);
	if(editing){
		getSprite()->fillRoundRect(getTotalX() + 100 + movingCursor, getTotalY() + 10, 4, 9, 1, TFT_RED);
	}else{
		getSprite()->fillRoundRect(getTotalX() + 100 + movingCursor, getTotalY() + 11, 4, 7, 1, TFT_WHITE);
	}
	getSprite()->setTextColor(TFT_WHITE);
	if(index == 0){
		getSprite()->setCursor(getTotalX() + 120, getTotalY() + 2);
		getSprite()->println("OFF");
		return;
	}else if(shutDownTime[index] >= 60){
		getSprite()->setCursor(getTotalX() + 115, getTotalY() + 2);
		getSprite()->println(shutDownTime[index] / 60);
		getSprite()->setCursor(getTotalX() + 127, getTotalY() + 2);
		getSprite()->println("min");
	}else{
		getSprite()->setCursor(getTotalX() + 112, getTotalY() + 2);
		getSprite()->println(shutDownTime[index]);
		getSprite()->setCursor(getTotalX() + 124, getTotalY() + 2);
		getSprite()->println(" sec");
	}

}

bool SettingsScreen::DiscreteSlider::isSliderSelected() const{
	return editing;
}

void SettingsScreen::DiscreteSlider::setIndex(int index){
	DiscreteSlider::index = index;

}

int SettingsScreen::DiscreteSlider::getIndex() const{
	return index;

}


