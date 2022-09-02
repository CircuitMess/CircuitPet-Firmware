
#include <Loop/LoopManager.h>
#include <SPIFFS.h>
#include "SplashState.h"
#include "Home/Menu/Menu.h"
#include "Input/Input.h"
#include <FS/CompressedFile.h>


SplashState::SplashState(Sprite* base, const MenuItemData& menuItem) : base(base), instructPath(menuItem.instructPath), splashPath(menuItem.splashPath){
	game = menuItem.primary();
}

void SplashState::onStart(){
	File splash = CompressedFile::open(SPIFFS.open(splashPath), 10, 5);
	base->drawIcon(splash, 0, 0, 160, 128);

	startTime = millis();
	LoopManager::resetTime();
	LoopManager::addListener(this);
	Input::getInstance()->addListener(this);
}

void SplashState::onStop(){
	Input::getInstance()->removeListener(this);
	LoopManager::removeListener(this);
}

void SplashState::loop(uint micros){
	if(!startedLoading){
		startedLoading = true;
		game->load();
	}

	if(!splashDone && game->isLoaded() && (millis() - startTime >= minSplashTime)){
		splashDone = true;
		File instruct = CompressedFile::open(SPIFFS.open(instructPath), 10, 5);
		base->drawIcon(instruct, 0, 0, 160, 128);
	}
}

void SplashState::buttonPressed(uint i){
	if(!splashDone) return;

	game->setParent(getParent());
	volatile auto game = this->game;

	stop();
	delete this;

	LoopManager::resetTime();
	game->start();
}
