
#ifndef CIRCUITPET_FIRMWARE_COLLISIONSYSTEM_H
#define CIRCUITPET_FIRMWARE_COLLISIONSYSTEM_H

#include "../Games/GameObject.h"
#include "../Games/Game.h"
#include <functional>
#include <vector>

struct CollisionPair {
	std::shared_ptr<const GameObject> first;
	std::shared_ptr<const GameObject> second;
	std::function<void()> handler;
};



class CollisionSystem {
public:
	void addPair(std::shared_ptr<const GameObject> first, std::shared_ptr<const GameObject> second, std::function<void()> handler);
protected:
	friend Game;
private:
	std::vector<CollisionPair> pairs;
	void removeObject(std::shared_ptr<const GameObject> GO);
};


#endif //CIRCUITPET_FIRMWARE_COLLISIONSYSTEM_H
