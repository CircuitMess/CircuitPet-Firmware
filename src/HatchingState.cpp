#include "HatchingState.h"
#include <SPIFFS.h>
#include "Home/DuckScreen.h"
#include <Loop/LoopManager.h>
#include <FS/RamFile.h>
#include <FS/CompressedFile.h>
#include "Stats/StatsManager.h"

HatchingState::HatchingState(Sprite* base) : gif(base, SPIFFS.open("/hatching.gif")), base(base){

}

void HatchingState::onStart(){
	gif.start();
	gif.setLoopMode(GIF::Single);
	gif.setLoopDoneCallback([this](uint32_t){
		exit = true;
	});
	LoopManager::addListener(this);
}

void HatchingState::onStop(){
	gif.stop();
	LoopManager::removeListener(this);
}

void HatchingState::loop(uint micros){
	if(exit){
		volatile auto temp = base;
		StatMan.setHatched(true);
		stop();
		delete this;
		auto duck = new DuckScreen(temp);
		LoopManager::loop();

		duck->start();
		return;
	}

	gif.push();
}
