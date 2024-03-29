#include "HatchingState.h"
#include <SPIFFS.h>
#include "Home/DuckScreen.h"
#include <Loop/LoopManager.h>
#include <FS/RamFile.h>
#include "Stats/StatsManager.h"
#include <Input/Input.h>

HatchingState::HatchingState(Sprite* base) : gif(base, SPIFFS.open("/hatching.gif")), base(base){

}

void HatchingState::onStart(){
	LoopManager::resetTime();

	gif.start();
	gif.stop();
	gif.setLoopMode(GIF::Single);
	gif.setLoopDoneCallback([this](uint32_t){
		exit = true;
	});
	LoopManager::addListener(this);
	Input::getInstance()->addListener(this);
}

void HatchingState::onStop(){
	StatMan.reset();
	StatMan.setHatched(true);
	gif.stop();
	LoopManager::removeListener(this);
}

void HatchingState::loop(uint micros){
	if(exit){
		volatile auto temp = base;
		stop();
		delete this;
		auto duck = new DuckScreen(temp);

		duck->start();
		return;
	}

	gif.push();

	if(!accepted){
		blinkTime += micros;
		if(blinkTime >= 1000000) blinkTime = 0;
		if(micros % 1000000 < 750000){
			File frame = SPIFFS.open("/frame.raw");
			base->drawIcon(frame, 16, 16, 128, 96, 1, TFT_TRANSPARENT);

			base->setTextColor(TFT_BLACK);
			base->setCursor(40, 46);
			base->print("Press any key");
			base->setCursor(48, 59);
			base->print("to receive");
			base->setCursor(32, 72);
			base->print("your CircuitPet!");
		}
	}
}

void HatchingState::buttonPressed(uint i){
	if(!accepted){
		Input::getInstance()->removeListener(this);
		accepted = true;
		gif.start();
	}
}
