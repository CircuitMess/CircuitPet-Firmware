#include "DeathState.h"
#include <SPIFFS.h>
#include "HatchingState.h"
#include <Loop/LoopManager.h>
#include <FS/RamFile.h>
#include "Stats/StatsManager.h"

DeathState* DeathState::instance = nullptr;

DeathState::DeathState(Sprite* base) : explosion(base, SPIFFS.open("/death.g565")), bg(base, StatMan.getLevel()),
									   duck(base, SPIFFS.open(paths[DeathState::getLevel()])), base(base){
	instance = this;
}

void DeathState::onStart(){
	duck.setXY(50, 36);
	duck.start();
	duck.stop();

	explosion.setLoop(false);
	explosion.setMaskingColor(TFT_BLACK);
	explosion.setLoopDoneCallback([](){
		instance->exit = true;
	});
	LoopManager::addListener(this);
}

void DeathState::onStop(){
	explosion.stop();
	LoopManager::removeListener(this);
}

void DeathState::loop(uint micros){


	bg.push();

	if(duckVisible){
		duck.push();
	}

	if(!exploded){
		explosionMicros += micros;
		if(explosionMicros > explosionStart){
			exploded = true;
			explosion.start();
			explosion.reset();
		}
	}else{

		duckRemoveMicros += micros;
		if(duckRemoveMicros >= duckVisibleMicros){
			duckVisible = false;
		}

		if(explosion.checkFrame()){
			explosion.nextFrame();
		}
		explosion.push();
	}

	if(exit){
		exitMicros += micros;
		if(exitMicros >= 800000){
			volatile auto temp = base;
			stop();
			delete this;
			auto screen = new HatchingState(temp);
			screen->start();
			return;
		}
	}
}

uint8_t DeathState::getLevel(){
	if(StatMan.getLevel() == 6){
		return 6;
	}else if(StatMan.getLevel() >= 4){
		return 4;
	}else{
		return 1;
	}
}
