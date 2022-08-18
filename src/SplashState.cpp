
#include <Loop/LoopManager.h>
#include <SPIFFS.h>
#include "SplashState.h"
#include "Home/Menu/Menu.h"
#include "Input/Input.h"
#include <FS/CompressedFile.h>


SplashState::SplashState(Sprite* base, const MenuItem& menuItem) : base(base), instructPath(menuItem.instructPath), splashPath(menuItem.splashPath){
	game = menuItem.primary();
}

void SplashState::onStart(){
	game->load();
	startTime = millis();
	LoopManager::resetTime();
	LoopManager::addListener(this);
	Input::getInstance()->addListener(this);

	File splash = CompressedFile::open(SPIFFS.open(splashPath), 10, 5);
	base->drawIcon(splash, 0, 0, 160, 128);
	base->push();
	splash.close();
}

void SplashState::onStop(){
	Input::getInstance()->removeListener(this);
	LoopManager::removeListener(this);
}

void SplashState::loop(uint micros){
	if(!splashDone && game->isLoaded() && (millis() - startTime >= minSplashTime)){
		splashDone = true;
		File instruct = CompressedFile::open(SPIFFS.open(instructPath), 10, 5);
		base->drawIcon(instruct, 0, 0, 160, 128);
		base->push();
		instruct.close();
	}
}

void SplashState::buttonPressed(uint i){
	if(!splashDone) return;
	Game* game = this->game;
	State* p = this->getParent();
	stop();
	delete this;

	LoopManager::resetTime();
	game->push(p);

}
