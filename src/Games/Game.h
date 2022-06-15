#ifndef CIRCUITPET_FIRMWARE_GAME_H
#define CIRCUITPET_FIRMWARE_GAME_H

#include "ResourceManager.h"
#include "GameObject.h"
#include <Util/Task.h>
#include <set>

class Game {
public:
	Game(const char* root, std::vector<ResDescriptor> resources);
	void load();
	bool isLoaded() const;

protected:
	virtual void onLoad() = 0;

	File getFile(std::string path);

	void addObject(const std::shared_ptr<GameObject>& obj);
	void removeObject(const std::shared_ptr<GameObject>& obj);

private:
	ResourceManager resMan;
	const std::vector<ResDescriptor> resources;

	bool loaded = false;
	Task loadTask;


	std::set<std::shared_ptr<GameObject>> objects;

};


#endif //CIRCUITPET_FIRMWARE_GAME_H
