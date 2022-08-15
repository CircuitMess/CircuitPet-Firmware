#include "Intro.h"
#include <SPIFFS.h>
#include "HatchingState.h"
#include "Home/DuckScreen.h"
#include "Stats/StatsManager.h"
#include <Loop/LoopManager.h>
#include "DeathState.h"
#include <FS/CompressedFile.h>

Intro* Intro::instance = nullptr;
Intro::Intro(Sprite* base) : gif(base, CompressedFile::open(SPIFFS.open("/intro.hs"), 10, 5)), base(base){
	instance = this;
}

void Intro::onStart(){
	gif.start();
	gif.setLoop(false);
	gif.setLoopDoneCallback([](){
		instance->exit = true;
	});

	LoopManager::addListener(this);
}



void Intro::onStop(){
	gif.stop();
	LoopManager::removeListener(this);
}

void Intro::loop(uint micros){
	if(exit){
		delay(500);
		volatile auto temp = instance->base;
		instance->stop();
		delete instance;


		if(StatMan.isHatched()){
			if(StatMan.hasDied()){
				auto duck = new DeathState(temp);
				duck->start();
			}else{
				auto duck = new DuckScreen(temp);
				duck->start();
			}
		}else{
			auto hatch = new HatchingState(temp);
			hatch->start();
		}

		return;
	}


	if(gif.checkFrame()){
		gif.nextFrame();
	}
	gif.push();
}
