
#ifndef CIRCUITPET_FIRMWARE_COLLISIONSYSTEM_H
#define CIRCUITPET_FIRMWARE_COLLISIONSYSTEM_H

#include "../GameObject.h"
#include "../Game.h"
#include <functional>
#include <vector>

struct CollisionPair {
	std::shared_ptr<const GameObject> first;
	std::shared_ptr<const GameObject> second;
	std::function<void()> handler;
};

class CollisionSystem : public GameSystem {
friend Game;
public:
	void collide();
	void addPair(std::shared_ptr<const GameObject> first, std::shared_ptr<const GameObject> second, std::function<void()> handler);

private:
	std::vector<CollisionPair> pairs;
	void removeObject(std::shared_ptr<const GameObject> GO);

	static bool circleCircle(const GameObject& circle1, const GameObject& circle2);
	static bool rectRect(const GameObject& square1, const GameObject& square2);
	static bool rectCircle(const GameObject& rect, const GameObject& circle);
};


#endif //CIRCUITPET_FIRMWARE_COLLISIONSYSTEM_H
