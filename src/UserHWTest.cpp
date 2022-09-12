#include <vector>
#include <Pins.hpp>
#include "UserHWTest.h"
#include <Input/Input.h>
#include <Loop/LoopManager.h>
#include <Audio/Piezo.h>
#include "Notes.h"
#include <SPIFFS.h>
#include <FS/RamFile.h>
#include <CircuitPet.h>

const char* labels[3] = {"Press buttons to test them.",
						 "You should hear a melody playing on the buzzer.\n\nPress any key to continue.",
						 "The LED light should glow red, green and blue.\n\nPress any key to exit test."};

const std::unordered_map<uint8_t, uint8_t> UserHWTest::KeyMap = {
		{ BTN_LEFT,  0},
		{ BTN_RIGHT, 1},
		{ BTN_ENTER, 2},
		{ BTN_BACK,  3}};

const std::array<PixelDim, 4> UserHWTest::KeyPos = {
		PixelDim { 18, 79 },
		PixelDim { 31, 81 },
		PixelDim { 44, 81 },
		PixelDim { 58, 79 }
};

const std::vector<UserHWTest::Note> UserHWTest::Notes = {
		{ NOTE_C4, 400 },
		{ NOTE_E4, 400 },
		{ NOTE_G4, 400 },
		{ NOTE_C5, 600 },
		{ NOTE_C5, 400 },
		{ NOTE_G4, 400 },
		{ NOTE_E4, 400 },
		{ NOTE_C4, 600 }
};

UserHWTest::UserHWTest(Sprite* base, std::function<void()> doneFunc) : base(base), doneFunc(doneFunc){
	for(int i = 0; i < 4; i++){
		inputTested[i] = false;
	}

	bg = SPIFFS.open("/Games/6/bg.raw");
	device = SPIFFS.open("/Test.raw");

	base->setTextColor(C_RGB(182, 213, 60));
	base->drawCenterString("Hardware Test", base->width() / 2, 1, &lgfx::fonts::Font0);
}

void UserHWTest::onStart(){
	Input::getInstance()->addListener(this);
	draw();
}

void UserHWTest::onStop(){
	Input::getInstance()->removeListener(this);
	LoopManager::removeListener(this);
	Piezo.noTone();
}

void UserHWTest::draw(){
	base->drawIcon(bg, 0, 0, 160, 128);
	base->drawIcon(device, 80, 20, 78, 89, 1, TFT_TRANSPARENT);

	base->setTextFont(0);
	base->setTextColor(C_RGB(182, 213, 60));
	base->setTextSize(0);
	base->setCursor(42, 2);
	base->print("Hardware Test");

	for(int i = 0; i < 4; i++){
		if(inputTested[i]){
			base->fillCircle(KeyPos[i].x + 80, KeyPos[i].y + 20, 5, TFT_GREEN);
		}
	}

	Sprite textSprite(base, 80, 120);
	textSprite.setPos(3, 26);
	textSprite.clear(TFT_TRANSPARENT);

	textSprite.setTextFont(0);
	textSprite.setTextSize(0);
	textSprite.setTextColor(TFT_WHITE);

	if(stage == 0){
		textSprite.print("Press the\n");
		textSprite.print("buttons to\n");
		textSprite.print("test them.");
	}else if(stage == 1){
		textSprite.print("The RGB LED\n");
		textSprite.print("should light\n");
		textSprite.print("up red,\n");
		textSprite.print("green, and\n");
		textSprite.print("blue.\n\n");
		textSprite.print("Press any\n");
		textSprite.print("key to\n");
		textSprite.print("continue.");
	}else if(stage == 2){
		textSprite.print("You should\n");
		textSprite.print("hear a\n");
		textSprite.print("melody\n");
		textSprite.print("playing.\n\n");
		textSprite.print("Press any\n");
		textSprite.print("key if\n");
		textSprite.print("everything\n");
		textSprite.print("is OK.");
	}

	textSprite.push();
}

void UserHWTest::buttonPressed(uint i){
	if(stage == 0){
		auto pair = KeyMap.find(i);
		if(pair == KeyMap.end()) return;
		inputTested[pair->second] = true;
		draw();

		int tested = 0;
		for(int i = 0; i < 4; i++){
			tested += inputTested[i];
		}
		if(tested == 4){
			stage2();
			draw();
		}
	}else if(stage == 1){
		RGB.setColor(Pixel::Black);
		stage3();
		draw();
	}else if(stage == 2){
		if(doneFunc){
			stop();
			auto func = doneFunc;
			delete this;
			func();
		}else{
			pop();
		}
	}
}

void UserHWTest::stage2(){
	stage = 1;
	colorTime = millis();
	RGB.setColor(Pixel::Red);
	LoopManager::addListener(this);
}

void UserHWTest::stage3(){
	stage = 2;

	noteIndex = 0;
	noteTime = millis();
	Piezo.tone(Notes[noteIndex].freq);
}

void UserHWTest::loop(uint micros){
	if(stage == 1){
		if(millis() - colorTime <= 500) return;
		colorTime = millis();
		colorIndex = (colorIndex + 1) % 3;

		if(colorIndex == 0) RGB.setColor(Pixel::Red);
		else if(colorIndex == 1) RGB.setColor(Pixel::Green);
		else if(colorIndex == 2) RGB.setColor(Pixel::Blue);
	}if(stage == 2){
		if(millis() - noteTime < Notes[noteIndex].duration) return;

		noteIndex = (noteIndex + 1) % Notes.size();
		noteTime = millis();
		Piezo.tone(Notes[noteIndex].freq);
	}
}
