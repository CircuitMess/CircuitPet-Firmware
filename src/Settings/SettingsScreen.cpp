#include "SettingsScreen.h"
//#include "../UserHWTest/UserHWTest.h"
#include <Input/Input.h>
#include <FS/CompressedFile.h>
#include <Settings.h>
#include <SPIFFS.h>
#include <Pins.hpp>
#include <CircuitPet.h>
#include <Loop/LoopManager.h>

SettingsScreen::SettingsScreen* SettingsScreen::SettingsScreen::instance = nullptr;

SettingsScreen::SettingsScreen::SettingsScreen(Display& display) : screen(display), screenLayout(new LinearLayout(&screen, VERTICAL)),
																   shutDownSlider(new DiscreteSlider(screenLayout, "Auto shutdown", { 0, 1, 5, 15, 30 })),
																   brightnessSlider(new SliderElement(screenLayout, "Brightness")),
																   soundSwitch(new BooleanElement(screenLayout, "Sound")),
																   enableLED(new BooleanElement(screenLayout, "LED enable")),
																   inputTest(new TextElement(screenLayout, "Hardware test")),
																   save(new TextElement(screenLayout, "Save")),
																   elements({ shutDownSlider, brightnessSlider, soundSwitch, enableLED, inputTest, save }){
	instance = this;
	buildUI();
	shutDownSlider->setIsSelected(true);
	shutDownSlider->setIndex(Settings.get().shutdownTime);
	brightnessSlider->setSliderValue(Settings.get().screenBrightness);
	soundSwitch->setBooleanSwitch(Settings.get().sound);
	enableLED->setBooleanSwitch(Settings.get().RGBenable);

	screen.pack();
}

void SettingsScreen::SettingsScreen::onStart(){
	screen.unpack();

	Input::getInstance()->addListener(this);
	Input::getInstance()->setButtonHeldRepeatCallback(BTN_RIGHT, 150, [](uint){
		if(instance == nullptr || instance->selectedSetting != 1) return;
		instance->brightnessSlider->moveSliderValue(instance->scrollStep);

		Settings.get().screenBrightness = instance->brightnessSlider->getSliderValue();
		CircuitPet.setBrightness(instance->brightnessSlider->getSliderValue());

		instance->scrollCount++;
		if(instance->scrollCount >= 3){
			instance->scrollCount = 0;
			instance->scrollStep *= 2;
		}


		instance->draw();
	});

	Input::getInstance()->setButtonHeldRepeatCallback(BTN_LEFT, 150, [](uint){
		if(instance == nullptr || instance->selectedSetting != 1) return;
		instance->brightnessSlider->moveSliderValue(-instance->scrollStep);

		Settings.get().screenBrightness = instance->brightnessSlider->getSliderValue();
		CircuitPet.setBrightness(instance->brightnessSlider->getSliderValue());

		instance->scrollCount++;
		if(instance->scrollCount >= 3){
			instance->scrollCount = 0;
			instance->scrollStep *= 2;
		}


		instance->draw();
	});
	backgroundBuffer = static_cast<Color*>(malloc(160 * 128 * 2));
	if(backgroundBuffer == nullptr){
		Serial.println("SettingsScreen background unpack error");
		return;
	}

	fs::File bgFile = SPIFFS.open("/Bg/settings.raw");
	bgFile.read(reinterpret_cast<uint8_t*>(backgroundBuffer), 160 * 128 * 2);
	bgFile.close();

	draw();
}

void SettingsScreen::SettingsScreen::onStop(){
	Input::getInstance()->removeListener(this);
	LoopManager::removeListener(this);
	Input::getInstance()->removeButtonHeldRepeatCallback(BTN_RIGHT);
	Input::getInstance()->removeButtonHeldRepeatCallback(BTN_LEFT);
	RGB.setColor({ 0, 0, 0 });

	free(backgroundBuffer);
	screen.pack();
}

void SettingsScreen::SettingsScreen::draw(){
	screen.getSprite()->drawIcon(backgroundBuffer, 0, 0, 160, 128, 1);
//	screen.getSprite()->setTextColor(TFT_WHITE);
//	screen.getSprite()->setTextSize(1);
//	screen.getSprite()->setTextFont(1);
//	screen.getSprite()->setCursor(screenLayout->getTotalX() + 42, screenLayout->getTotalY() + 110);
//	screen.getSprite()->print("Version 1.0");

	for(int i = 0; i < 6; i++){
		if(!reinterpret_cast<SettingsElement*>(screenLayout->getChild(i))->isSelected()){
			screenLayout->getChild(i)->draw();
		}
	}
	for(int i = 0; i < 6; i++){
		if(reinterpret_cast<SettingsElement*>(screenLayout->getChild(i))->isSelected()){
			screenLayout->getChild(i)->draw();
		}
	}
	screen.commit();
}

SettingsScreen::SettingsScreen::~SettingsScreen(){
	instance = nullptr;
}

void SettingsScreen::SettingsScreen::buildUI(){
	screenLayout->setWHType(PARENT, PARENT);
	screenLayout->setGutter(4);

	for(auto& el : elements){
		screenLayout->addChild(el);
	}

	screenLayout->reflow();
	screen.addChild(screenLayout);
	screen.repos();
}

void SettingsScreen::SettingsScreen::buttonPressed(uint id){
	switch(id){
		case BTN_LEFT:
			if(editMode){
				if(selectedSetting == 0){
					shutDownSlider->selectPrev();
				}else if(selectedSetting == 1){
					brightnessSlider->moveSliderValue(-1);
					Settings.get().screenBrightness = brightnessSlider->getSliderValue();
					CircuitPet.setBrightness(brightnessSlider->getSliderValue());
				}
			}else{
				elements[selectedSetting]->setIsSelected(false);
				selectedSetting--;
				if(selectedSetting < 0){
					selectedSetting = elements.size() - 1;
				}
				elements[selectedSetting]->setIsSelected(true);
			}
			break;

		case BTN_RIGHT:
			if(editMode){
				if(selectedSetting == 0){
					shutDownSlider->selectNext();
				}else if(selectedSetting == 1){
					brightnessSlider->moveSliderValue(1);
					Settings.get().screenBrightness = brightnessSlider->getSliderValue();
					CircuitPet.setBrightness(brightnessSlider->getSliderValue());
				}
			}else{
				elements[selectedSetting]->setIsSelected(false);
				selectedSetting++;
				if(selectedSetting > elements.size() - 1){
					selectedSetting = 0;
				}
				elements[selectedSetting]->setIsSelected(true);
			}
			break;

		case BTN_A:
			if(selectedSetting == 0 || selectedSetting == 1){
				editMode = !editMode;
			}
			elements[selectedSetting]->toggle();
			if(selectedSetting == 2){
				Settings.get().sound = instance->soundSwitch->getBooleanSwitch();
//				Playback.updateGain();
//				Playback.tone(500, 50);
			}else if(selectedSetting == 3){
				Settings.get().RGBenable = enableLED->getBooleanSwitch();
				if(!Settings.get().RGBenable){
					RGB.setColor(Pixel::Black);
				}else{
					RGB.setColor(Pixel::White);
					instance->blinkTime = millis();
					LoopManager::addListener(this);
				}
			}else if(selectedSetting == 4){
//				Context* hwTest = new UserHWTest(*CircuitPet.getDisplay());
//				hwTest->push(this);
//				draw();
				break;
			}else if(selectedSetting == 5){
				Settings.get().shutdownTime = shutDownSlider->getIndex();
				Settings.get().sound = soundSwitch->getBooleanSwitch();
				Settings.get().RGBenable = enableLED->getBooleanSwitch();
				Settings.store();
//				Playback.updateGain();
				pop();
				return;
			}
			break;

		case BTN_B:
			if(editMode){
				editMode = false;
				elements[selectedSetting]->toggle();
			}else{
				Settings.get().shutdownTime = shutDownSlider->getIndex();
				Settings.get().sound = soundSwitch->getBooleanSwitch();
				Settings.get().RGBenable = enableLED->getBooleanSwitch();
				Settings.store();
//			Playback.updateGain();
				pop();
				return;
			}
			break;
	}

	draw();
}

void SettingsScreen::SettingsScreen::loop(uint micros){
	if(blinkTime != 0 && millis() - blinkTime >= 200){
		blinkTime = 0;
		RGB.setColor(Pixel::Black);
		LoopManager::removeListener(this);
	}
}

void SettingsScreen::SettingsScreen::buttonReleased(uint id){
	if(id == BTN_LEFT || id == BTN_RIGHT){
		scrollStep = 1;
		scrollCount = 0;
	}

}
