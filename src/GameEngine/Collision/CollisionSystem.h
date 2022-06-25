
#ifndef CIRCUITPET_FIRMWARE_COLLISIONSYSTEM_H
#define CIRCUITPET_FIRMWARE_COLLISIONSYSTEM_H

#include <functional>
#include <vector>
#include "../GameSystem.h"

class CollisionSystem : public GameSystem {
friend Game;
public:
	CollisionSystem(const Game* game);

	void addPair(const GameObject& first, const GameObject& second, std::function<void()> handler);
	void removePair(const GameObject& first, const GameObject& second);

	void wallTop(const GameObject& obj, std::function<void()> handler);
	void wallBot(const GameObject& obj, std::function<void()> handler);
	void wallLeft(const GameObject& obj, std::function<void()> handler);
	void wallRight(const GameObject& obj, std::function<void()> handler);
	void wallsVertical(const GameObject& obj, std::function<void()> handler);
	void wallsHorizontal(const GameObject& obj, std::function<void()> handler);
	void wallsAll(const GameObject& obj, std::function<void()> handler);

	void drawDebug(Sprite* canvas);

	void update(uint32_t deltaMicros) override;

private:
	struct Pair {
		const GameObject* first;
		const GameObject* second;
		std::function<void()> handler;
		bool colliding = false;
	};

	struct {
		GameObject top;
		GameObject bot;
		GameObject left;
		GameObject right;
	} Walls;

	std::vector<Pair> pairs;
	void removeObject(const GameObject& GO);

	static bool circleCircle(const GameObject& circle1, const GameObject& circle2);
	static bool rectRect(const GameObject& square1, const GameObject& square2);
	static bool rectCircle(const GameObject& rect, const GameObject& circle);
};


#endif //CIRCUITPET_FIRMWARE_COLLISIONSYSTEM_H
