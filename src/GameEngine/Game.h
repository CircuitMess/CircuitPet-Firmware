#ifndef CIRCUITPET_FIRMWARE_GAME_H
#define CIRCUITPET_FIRMWARE_GAME_H

#include "ResourceManager.h"
#include "GameObject.h"
#include <Util/Task.h>
#include <set>
#include <vector>
#include "../State.h"
#include "GameSystem.h"
#include "Collision/CollisionSystem.h"
#include "Rendering/RenderSystem.h"
#include "../Stats/Stats.hpp"
#include <Loop/LoopListener.h>

class Game : public State, private LoopListener {
friend GameSystem;
public:
	void load();
	bool isLoaded() const;

	void start() final;
	void stop() final;
	void pop() final;

protected:
	Game(const char* root, std::vector<ResDescriptor> resources);

	virtual void onStart() override;
	virtual void onStop() override;
	virtual void onLoad();
	virtual void onLoop(float deltaTime);
	virtual void onRender(Sprite* canvas);
	virtual void setScore(uint8_t oil, uint8_t happiness) = 0;
	Stats returnStats = {0,0,0};

	File getFile(std::string path);

	void addObject(std::shared_ptr<GameObject> obj);
	void removeObject(std::shared_ptr<GameObject> obj);

	CollisionSystem collision;

private:
	ResourceManager resMan;
	const std::vector<ResDescriptor> resources;

	bool loaded = false;
	Task loadTask;

	volatile bool popped = false;

	RenderSystem render;

	std::set<std::shared_ptr<GameObject>> objects;

	void loop(uint micros) final;
	void loadFunc();
};


#endif //CIRCUITPET_FIRMWARE_GAME_H
