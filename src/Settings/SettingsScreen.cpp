#include "SettingsScreen.h"
#include "../UserHWTest.h"
//#include "../UserHWTest/UserHWTest.h"
#include <Input/Input.h>
#include <FS/CompressedFile.h>
#include <Settings.h>
#include <SPIFFS.h>
#include <Pins.hpp>
#include <CircuitPet.h>
#include <Loop/LoopManager.h>
#include <nvs_flash.h>
#include <Audio/Piezo.h>
#include <FS/RamFile.h>

SettingsScreen::SettingsScreen* SettingsScreen::SettingsScreen::instance = nullptr;

SettingsScreen::SettingsScreen::SettingsScreen(Display& display) : screen(display), scrollLayout(new ScrollLayout(&screen)),
																   screenLayout(new LinearLayout(scrollLayout, VERTICAL)),
																   shutDownSlider(new DiscreteSlider(screenLayout, "Sleep time", { 0, 15, 30, 60, 3 * 60 })),
																   brightnessSlider(new SliderElement(screenLayout, "Brightness")),
																   soundSwitch(new BooleanElement(screenLayout, "Sound")),
																   rgbSlider(new SliderElement(screenLayout, "LED brightness")),
																   hwTest(new TextElement(screenLayout, "Hardware test")),
																   factoryReset(new TextElement(screenLayout, "Factory reset")),
																   save(new TextElement(screenLayout, "Save")),
																   elements({ shutDownSlider, brightnessSlider, soundSwitch, rgbSlider, hwTest, factoryReset, save }){
	instance = this;
	buildUI();
	shutDownSlider->setIsSelected(true);
	shutDownSlider->setIndex(Settings.get().shutdownTime);
	brightnessSlider->setSliderValue(Settings.get().screenBrightness);
	soundSwitch->setBooleanSwitch(Settings.get().sound);
	rgbSlider->setSliderValue(Settings.get().RGBbrightness);
	screen.pack();
}

void SettingsScreen::SettingsScreen::onStart(){
	screen.unpack();

	Input::getInstance()->addListener(this);
	Input::getInstance()->setButtonHeldRepeatCallback(BTN_RIGHT, 150, [](uint){
		if(instance == nullptr) return;
		if(instance->selectedSetting == 1 && instance->editMode){
			Audio.play({{500, 500, 50}});

			instance->brightnessSlider->moveSliderValue(instance->scrollStep);

			Settings.get().screenBrightness = instance->brightnessSlider->getSliderValue();
			CircuitPet.setBrightness(instance->brightnessSlider->getSliderValue());

			instance->scrollCount++;
			if(instance->scrollCount >= 3){
				instance->scrollCount = 0;
				instance->scrollStep *= 2;
			}
		}else if(instance->selectedSetting == 3 && instance->editMode){
			Audio.play({{500, 500, 50}});

			instance->rgbSlider->moveSliderValue(instance->scrollStep);

			Settings.get().RGBbrightness = instance->rgbSlider->getSliderValue();
			RGB.setBrightness(instance->rgbSlider->getSliderValue());

			instance->scrollCount++;
			if(instance->scrollCount >= 3){
				instance->scrollCount = 0;
				instance->scrollStep *= 2;
			}
		}


		instance->draw();
	});

	Input::getInstance()->setButtonHeldRepeatCallback(BTN_LEFT, 150, [](uint){
		if(instance == nullptr) return;
		if(instance->selectedSetting == 1){
			Audio.play({{500, 500, 50}});

			instance->brightnessSlider->moveSliderValue(-instance->scrollStep);

			Settings.get().screenBrightness = instance->brightnessSlider->getSliderValue();
			CircuitPet.setBrightness(instance->brightnessSlider->getSliderValue());

			instance->scrollCount++;
			if(instance->scrollCount >= 3){
				instance->scrollCount = 0;
				instance->scrollStep *= 2;
			}
		}else if(instance->selectedSetting == 3){
			Audio.play({{500, 500, 50}});

			instance->rgbSlider->moveSliderValue(-instance->scrollStep);

			Settings.get().RGBbrightness = instance->rgbSlider->getSliderValue();
			RGB.setBrightness(instance->rgbSlider->getSliderValue());

			instance->scrollCount++;
			if(instance->scrollCount >= 3){
				instance->scrollCount = 0;
				instance->scrollStep *= 2;
			}
		}


		instance->draw();
	});

	bg = RamFile::open(SPIFFS.open("/Bg/settings.raw"));

	draw();
}

void SettingsScreen::SettingsScreen::onStop(){
	Input::getInstance()->removeListener(this);
	LoopManager::removeListener(this);
	Input::getInstance()->removeButtonHeldRepeatCallback(BTN_RIGHT);
	Input::getInstance()->removeButtonHeldRepeatCallback(BTN_LEFT);
	RGB.setColor({ 0, 0, 0 });

	screen.pack();
	bg.close();
}

void SettingsScreen::SettingsScreen::draw(){
	screen.getSprite()->drawIcon(bg, 0, 0, 160, 128, 1);
//	screen.getSprite()->setTextColor(TFT_WHITE);
//	screen.getSprite()->setTextSize(1);
//	screen.getSprite()->setTextFont(1);
//	screen.getSprite()->setCursor(screenLayout->getTotalX() + 42, screenLayout->getTotalY() + 110);
//	screen.getSprite()->print("Version 1.0");

	scrollLayout->draw();

	if(resetConfirm){
		auto base = screen.getSprite();

		File frame = SPIFFS.open("/frame.raw");
		base->drawIcon(frame, 16, 16, 128, 96, 1, TFT_TRANSPARENT);

		base->setTextColor(TFT_BLACK);
		base->setCursor(42, 27);
		base->print("Are you sure?");
		base->setCursor(34, 42);
		base->print("This will reset");
		base->setCursor(52, 52);
		base->print("ALL data!");

		base->setCursor(26, 76);
		base->print("Confirm");

		base->setCursor(98, 76);
		base->print("Cancel");

		for(int i = 0; i < 4; i++){
			int dy = (i == 1 || i == 2) ? 4 : 0;

			if(i == 0){
				base->fillCircle(48 + i * 20, 94 + dy, 6, TFT_BLACK);
			}else{
				base->drawCircle(48 + i * 20, 94 + dy, 6, TFT_BLACK);
			}
		}
	}

	screen.commit();
}

SettingsScreen::SettingsScreen::~SettingsScreen(){
	instance = nullptr;
}

void SettingsScreen::SettingsScreen::buildUI(){
	scrollLayout->setWHType(PARENT, PARENT);
	scrollLayout->setHeight(screen.getHeight());
	screenLayout->setWHType(PARENT, CHILDREN);
	screenLayout->setGutter(3);

	for(auto& el : elements){
		screenLayout->addChild(el);
	}

	scrollLayout->reflow();
	screenLayout->reflow();
	scrollLayout->addChild(screenLayout);
	screen.addChild(scrollLayout);
	screen.repos();
}

void SettingsScreen::SettingsScreen::buttonPressed(uint id){
	if(resetConfirm){
		if(id == BTN_LEFT){
			nvs_flash_erase();
			CircuitPet.fadeOut();
			ESP.restart();
		}else{
			resetConfirm = false;
			draw();
			return;
		}
	}

	switch(id){
		case BTN_LEFT:
			Audio.play({{500, 500, 50}});
			if(editMode){
				if(selectedSetting == 0){
					shutDownSlider->selectPrev();
				}else if(selectedSetting == 1){
					brightnessSlider->moveSliderValue(-1);
					Settings.get().screenBrightness = brightnessSlider->getSliderValue();
					CircuitPet.setBrightness(brightnessSlider->getSliderValue());
				}else if(selectedSetting == 3){
					rgbSlider->moveSliderValue(-1);
					Settings.get().RGBbrightness = rgbSlider->getSliderValue();
					RGB.setBrightness(rgbSlider->getSliderValue());
				}
			}else{
				elements[selectedSetting]->setIsSelected(false);
				selectedSetting--;
				if(selectedSetting < 0){
					selectedSetting = elements.size() - 1;
				}
				elements[selectedSetting]->setIsSelected(true);
				scrollLayout->scrollIntoView(selectedSetting, 0);
			}
			break;

		case BTN_RIGHT:
			Audio.play({{500, 500, 50}});
			if(editMode){
				if(selectedSetting == 0){
					shutDownSlider->selectNext();
				}else if(selectedSetting == 1){
					brightnessSlider->moveSliderValue(1);
					Settings.get().screenBrightness = brightnessSlider->getSliderValue();
					CircuitPet.setBrightness(brightnessSlider->getSliderValue());
				}else if(selectedSetting == 3){
					rgbSlider->moveSliderValue(1);
					Settings.get().RGBbrightness = rgbSlider->getSliderValue();
					RGB.setBrightness(rgbSlider->getSliderValue());
				}
			}else{
				elements[selectedSetting]->setIsSelected(false);
				selectedSetting++;
				if(selectedSetting > elements.size() - 1){
					selectedSetting = 0;
				}
				elements[selectedSetting]->setIsSelected(true);
				scrollLayout->scrollIntoView(selectedSetting, 0);
			}
			break;

		case BTN_A:
			Audio.play({{500, 700, 50}});

			if(selectedSetting == 0 || selectedSetting == 1 || selectedSetting == 3){
				editMode = !editMode;
				if(selectedSetting == 3){
					cycleRGB = !cycleRGB;
					if(cycleRGB){
						LoopManager::addListener(this);
					}else{
						LoopManager::removeListener(this);
						RGB.setColor(Pixel::Black);
					}
				}
			}
			elements[selectedSetting]->toggle();
			if(selectedSetting == 2){
				Settings.get().sound = !instance->soundSwitch->getBooleanSwitch();
				Piezo.setMute(Settings.get().sound);
			}else if(selectedSetting == 4){
				auto* hwTest = new UserHWTest(CircuitPet.getDisplay()->getBaseSprite(), [this](){ start(); });
				hwTest->push(this);
				break;
			}else if(selectedSetting == 5){
				resetConfirm = true;
				draw();
				return;
			}else if(selectedSetting == 6){
				Settings.get().shutdownTime = shutDownSlider->getIndex();
				Settings.get().sound = soundSwitch->getBooleanSwitch();
				Settings.get().RGBbrightness = rgbSlider->getSliderValue();
				Settings.store();
				Piezo.setMute(!Settings.get().sound);
				popped = true;
				LoopManager::addListener(this);
				return;
			}
			break;

		case BTN_B:
			Audio.play(Sound { Chirp { 400, 350, 50 }});

			if(editMode){
				editMode = false;
				elements[selectedSetting]->toggle();
				cycleRGB = false;
				LoopManager::removeListener(this);
				RGB.setColor(Pixel::Black);
			}else{
				Settings.get().shutdownTime = shutDownSlider->getIndex();
				Settings.get().sound = soundSwitch->getBooleanSwitch();
				Settings.get().RGBbrightness = rgbSlider->getSliderValue();
				Settings.store();
				Piezo.setMute(!Settings.get().sound);
				popped = true;
				LoopManager::addListener(this);
				return;
			}
			break;
	}

	draw();
}


typedef struct {
	double r;       // a fraction between 0 and 1
	double g;       // a fraction between 0 and 1
	double b;       // a fraction between 0 and 1
} rgb;

typedef struct {
	double h;       // angle in degrees
	double s;       // a fraction between 0 and 1
	double v;       // a fraction between 0 and 1
} hsv;

rgb hsv2rgb(hsv in){
	double hh, p, q, t, ff;
	long i;
	rgb out;

	if(in.s <= 0.0){       // < is bogus, just shuts up warnings
		out.r = in.v;
		out.g = in.v;
		out.b = in.v;
		return out;
	}
	hh = in.h;
	if(hh >= 360.0) hh = 0.0;
	hh /= 60.0;
	i = (long)hh;
	ff = hh - i;
	p = in.v * (1.0 - in.s);
	q = in.v * (1.0 - (in.s * ff));
	t = in.v * (1.0 - (in.s * (1.0 - ff)));

	switch(i){
		case 0:
			out.r = in.v;
			out.g = t;
			out.b = p;
			break;
		case 1:
			out.r = q;
			out.g = in.v;
			out.b = p;
			break;
		case 2:
			out.r = p;
			out.g = in.v;
			out.b = t;
			break;

		case 3:
			out.r = p;
			out.g = q;
			out.b = in.v;
			break;
		case 4:
			out.r = t;
			out.g = p;
			out.b = in.v;
			break;
		case 5:
		default:
			out.r = in.v;
			out.g = p;
			out.b = q;
			break;
	}
	return out;
}

void SettingsScreen::SettingsScreen::loop(uint micros){
	if(popped){
		pop();
		return;
	}
	double hue = 360.0f * ((double)((millis() % 2000) / 2000.0));

	hsv h = { hue, 1, 1 };
	rgb c = hsv2rgb(h);

	RGB.setColor({ (uint8_t)(c.r * 255), (uint8_t)(c.g * 255), (uint8_t)(c.b * 255) });

}

void SettingsScreen::SettingsScreen::buttonReleased(uint id){
	if(id == BTN_LEFT || id == BTN_RIGHT){
		scrollStep = 1;
		scrollCount = 0;
	}

}
