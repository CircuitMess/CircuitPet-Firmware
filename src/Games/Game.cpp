#include "Game.h"

#include <utility>

Game::Game(const char* root, std::vector<ResDescriptor> resources) : resMan(root), resources(std::move(resources)),
																	 loadTask("loadTask", [](Task* t){
																		 auto game = (Game*)t->arg;
																		 game->resMan.load(game->resources);
																		 game->loaded = true;
																	 }, 2048, this){}

void Game::load(){
	if(loaded || loadTask.running) return;

	loadTask.start(0, 0);
}

bool Game::isLoaded() const{
	return loaded;
}

File Game::getFile(std::string path){
	return resMan.getResource(std::move(path));
}

void Game::addObject(std::shared_ptr<GameObject> obj){
	objects.insert(obj);
}

void Game::removeObject(std::shared_ptr<GameObject> obj){
	objects.erase(obj);
}
