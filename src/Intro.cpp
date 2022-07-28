#include "Intro.h"
#include <SPIFFS.h>
#include "Home/DuckScreen.h"
#include <Loop/LoopManager.h>
#include <FS/RamFile.h>
#include <FS/CompressedFile.h>

Intro* Intro::instance = nullptr;
Intro::Intro(Sprite* base) : gif(base, CompressedFile::open(SPIFFS.open("/intro.hs"), 10, 5)), base(base){
	instance = this;
}

void Intro::onStart(){
	gif.start();
	gif.setLoop(false);
	gif.setLoopDoneCallback([](){
		auto duck = new DuckScreen(instance->base);
		instance->stop();
		delete instance;
		//TODO - start hatch instead of DuckScreen if necessary
		duck->start();
		return;
	});

	LoopManager::addListener(this);
}



void Intro::onStop(){
	gif.stop();
	LoopManager::removeListener(this);
}

void Intro::loop(uint micros){
	if(gif.checkFrame()){
		gif.nextFrame();
	}
	gif.push();
}
