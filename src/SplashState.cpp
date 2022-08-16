
#include <Loop/LoopManager.h>
#include <SPIFFS.h>
#include "SplashState.h"
#include "Home/Menu/Menu.h"
#include "Input/Input.h"
#include <FS/CompressedFile.h>


SplashState::SplashState(Sprite* base, const MenuItem& menuItem) : base(base){
	splash = CompressedFile::open(SPIFFS.open(menuItem.splashPath), 10, 5);
	instruct = CompressedFile::open(SPIFFS.open(menuItem.instructPath), 10, 5);
	game = menuItem.primary();
}

void SplashState::onStart(){
	game->load();
	startTime = millis();
	LoopManager::loop();
	LoopManager::addListener(this);
	Input::getInstance()->addListener(this);
	base->drawIcon(splash, 0, 0, 160, 128);
	base->push();
}

void SplashState::onStop(){
	Input::getInstance()->removeListener(this);
	LoopManager::removeListener(this);
}

void SplashState::loop(uint micros){
	if(!splashDone && game->isLoaded() && (millis() - startTime >= minSplashTime)){
		splashDone = true;
		base->drawIcon(instruct, 0, 0, 160, 128);
		base->push();
	}
}

void SplashState::buttonPressed(uint i){
	if(!splashDone) return;
	Game* game = this->game;
	State* p = this->getParent();
	stop();
	delete this;

	game->push(p);

}
