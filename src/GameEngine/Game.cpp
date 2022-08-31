#include "Game.h"

#include <utility>
#include <Loop/LoopManager.h>
#include <CircuitPet.h>
#include "../Stats/StatsManager.h"

Game::Game(const char* root, std::vector<ResDescriptor> resources) : resMan(root), resources(std::move(resources)),
loadTask("loadTask", [](Task* t){
	 auto game = (Game*) t->arg;
	 game->loadFunc();
}, 4096, this), render(this, CircuitPet.getDisplay()->getBaseSprite()), collision(this){

}

void Game::load(){
	if(loaded || loadTask.running) return;

	loadTask.start(0, 0);
}

void Game::loadFunc(){
	resMan.load(resources);
	onLoad();
	loaded = true;
}

void Game::start(){
	if(isStarted()) return;

	if(!loaded){
		ESP_LOGE("Game", "Attempting to load game that wasn't loaded.");
		return;
	}

	State::start();
	LoopManager::addListener(this);
}

void Game::stop(){
	if(!isStarted()) return;
	State::stop();
	LoopManager::removeListener(this);
}

bool Game::isLoaded() const{
	return loaded;
}

File Game::getFile(const char* path){
	return resMan.getResource(std::move(path));
}

void Game::addObject(std::shared_ptr<GameObject> obj){
	objects.insert(obj);
}

void Game::removeObject(std::shared_ptr<GameObject> obj){ 
	collision.removeObject(*obj);
	objects.erase(obj); 
}

void Game::loop(uint micros){
	if(popped) goto poppedLabel;

	collision.update(micros);
	onLoop((float)micros / 1000000.0f);
	if(popped) goto poppedLabel;

	render.update(micros);
	onRender(CircuitPet.getDisplay()->getBaseSprite());
	if(popped) goto poppedLabel;

	// collision.drawDebug(CircuitPet.getDisplay()->getBaseSprite());

	return;

	poppedLabel:
	auto stats = returnStats();
	State::pop();
	StatMan.update(stats);
}

void Game::onStart(){ }

void Game::onStop(){ }

void Game::onLoad(){ }

void Game::onLoop(float deltaTime){ }

void Game::onRender(Sprite* canvas){ }

void Game::pop(){
	popped = true;
}

Stats Game::returnStats(){
	return Stats({ 0, 0, 0 });
}
