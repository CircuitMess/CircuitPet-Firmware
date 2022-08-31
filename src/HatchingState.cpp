#include "HatchingState.h"
#include <SPIFFS.h>
#include "Home/DuckScreen.h"
#include <Loop/LoopManager.h>
#include <FS/RamFile.h>
#include "Stats/StatsManager.h"
#include <Input/Input.h>
#include <FS/CompressedFile.h>

HatchingState* HatchingState::instance = nullptr;

HatchingState::HatchingState(Sprite* base) : gif(base, CompressedFile::open(SPIFFS.open("/hatch.g565.hs"), 8, 4), false), base(base){
	instance = this;
}

void HatchingState::onStart(){
	LoopManager::resetTime();

	gif.setLoop(false);
	gif.setLoopDoneCallback([](){
		instance->exit = true;
	});
	gif.nextFrame();
	gif.push();
	base->push();
	gif.start();


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
	}else{
		if(gif.checkFrame()){
			gif.nextFrame();
		}
		gif.push();
	}
}

void HatchingState::buttonPressed(uint i){
	if(!accepted){
		Input::getInstance()->removeListener(this);
		accepted = true;
		gif.start();
	}
}
