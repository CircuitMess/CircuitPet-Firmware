#include <vector>
#include <Pins.hpp>
#include "UserHWTest.h"
#include <Input/Input.h>
#include <Loop/LoopManager.h>
#include <Audio/Piezo.h>
#include "Notes.h"
#include <SPIFFS.h>

const char* labels[3] = {"Press buttons to test them.",
						 "You should hear a melody playing on the buzzer.\n\nPress any key to continue.",
						 "The LED light should glow red, green and blue.\n\nPress any key to exit test."};

const std::unordered_map<uint8_t, UserHWTest::KeyCoord> UserHWTest::KeyCoords = {
		{ BTN_LEFT,  { 13, 74 }},
		{ BTN_RIGHT, { 28, 74 }},
		{ BTN_ENTER, { 43, 74 }},
		{ BTN_BACK,  { 58, 74 }}};

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

UserHWTest::UserHWTest(Sprite* base, std::function<void()> doneFunc) : base(base), doneFunc(doneFunc), bgSprite(base, 160, 128){

	bgSprite.drawIcon(SPIFFS.open("/Games/6/bg.raw"), 0, 0, 160, 128);


	bgSprite.push();
	base->setTextColor(C_RGB(182, 213, 60));
	base->drawCenterString("Hardware Test", base->width() / 2, 1, &lgfx::fonts::Font0);

/*	for(const char* text : std::vector<const char*> {
			"Press buttons to test them.",
			"You should hear a melody playing on the buzzer.\n\nPress any key to exit test."
	}){
		labels.push_back(lv_label_create(container));
		lv_obj_t* label = labels.back();

		lv_label_set_long_mode(label, LV_LABEL_LONG_WRAP);
		lv_label_set_text(label, text);
		lv_obj_set_size(label, lv_pct(100), LV_SIZE_CONTENT);
		lv_obj_set_style_text_font(label, &pixelbasic7, 0);
		lv_obj_set_style_text_line_space(label, 5, 0);
		lv_obj_set_style_text_color(label, lv_color_white(), 0);
		lv_obj_add_flag(label, LV_OBJ_FLAG_HIDDEN);
	}
	lv_obj_clear_flag(labels.front(), LV_OBJ_FLAG_HIDDEN);

	for(int i = 0; i < buttons.size(); i++){
		buttons[i] = false;
	}*/
}

void UserHWTest::onStart(){
	Input::getInstance()->addListener(this);
}

void UserHWTest::onStop(){
	Input::getInstance()->removeListener(this);
	LoopManager::removeListener(this);
	Piezo.noTone();
}

void UserHWTest::buttonPressed(uint i){
	if(stage == 1){
		onStop();

		if(doneFunc){
			stop();
			auto func = doneFunc;
			delete this;
			func();
		}else{
			pop();
		}
		return;
	}

	if(stage != 0) return;

	if(i >= buttons.size()) return;
	if(buttons[i]) return;

	auto pair = KeyCoords.find(i);
	if(pair == KeyCoords.end()) return;

	KeyCoord coord = pair->second;

	buttons[i] = true;
	buttonCount++;

	base->fillCircle(coord.x - 3, coord.y - 3, 7, TFT_GREEN);

	if(buttonCount == 4){
		startBuzz();
	}
}

void UserHWTest::startBuzz(){
	if(stage == 1) return;

	stage = 1;
//	lv_obj_add_flag(labels[0], LV_OBJ_FLAG_HIDDEN);
//	lv_obj_clear_flag(labels[1], LV_OBJ_FLAG_HIDDEN);

	LoopManager::addListener(this);
	noteIndex = 0;
	noteTime = millis();
	Piezo.tone(Notes[noteIndex].freq);
}

void UserHWTest::loop(uint micros){
	if(millis() - noteTime < Notes[noteIndex].duration) return;

	noteIndex = (noteIndex + 1) % Notes.size();
	noteTime = millis();
	Piezo.tone(Notes[noteIndex].freq);

}
