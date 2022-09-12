#include <Arduino.h>
#include <CircuitPet.h>
#include <Loop/LoopManager.h>
#include <CircuitOS.h>
#include <SPIFFS.h>
#include "src/Intro.h"
#include "src/Stats/StatsManager.h"
#include "src/Clock/ClockMaster.h"
#include <Settings.h>
#include "src/UserHWTest.h"
#include "src/JigHWTest/JigHWTest.h"
#include "src/AutoShutdown.h"
#include <Audio/Piezo.h>

extern "C" {
#include <bootloader_random.h>
}


bool checkJig(){
 //TODO - add jig detection
 return false;
}

Display* display;
Sprite* baseSprite;

void initLog(){
	esp_log_level_set("*", ESP_LOG_NONE);

	const static auto tags = { "*" };

	for(const char* tag : tags){
		esp_log_level_set(tag, ESP_LOG_VERBOSE);
	}
}

void setup(){
	Serial.begin(115200);
	initLog();

	bootloader_random_enable();
	srand(esp_random());
	bootloader_random_disable();

	LoopManager::reserve(12);

	CircuitPet.begin(false);
	Input* input = CircuitPet.getInput();

	display = CircuitPet.getDisplay();
	baseSprite = display->getBaseSprite();

	if(checkJig()){
		CircuitPet.fadeIn(0);
		printf("Jig\n");
		auto test = new JigHWTest(display);
		test->start();
		for(;;);
	}


	baseSprite->clear(TFT_BLACK);
	display->commit();

	baseSprite->setTextFont(0);
	baseSprite->setTextSize(0);


	Clock.begin();
	StatMan.begin();

	Clock.updatePersistent();

	auto shutdown = new AutoShutdown();
	shutdown->begin();

	if(!Settings.get().hwTested){
		auto test = new UserHWTest(baseSprite, [](){
			Settings.get().hwTested = true;
			Settings.store();
			CircuitPet.fadeOut();

			auto intro = new Intro(baseSprite);
			LoopManager::resetTime();
			intro->start();
			display->commit();
			CircuitPet.fadeIn();
		});
		test->start();
		CircuitPet.fadeIn();
	}else{
		Piezo.setMute(!Settings.get().sound);
		auto intro = new Intro(baseSprite);
		LoopManager::loop();
		intro->start();
		CircuitPet.fadeIn();
	}
}

uint32_t t = 0;
void loop(){
	LoopManager::loop();

	uint32_t t2 = micros();
	float frameTime = (float) (t2 - t) / 1000000.0f;
	baseSprite->setTextFont(0);
	baseSprite->setTextSize(0);
	baseSprite->setTextColor(TFT_WHITE);
	baseSprite->setCursor(1, 119);
	// baseSprite->printf("%.1fms - %.1ffps\n", frameTime * 1000.0f, 1.0f / frameTime);
	t = t2;

	display->commit();
}

