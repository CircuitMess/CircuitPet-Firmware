
#include "CollisionSystem.h"

void CollisionSystem::addPair(std::shared_ptr<const GameObject> first, std::shared_ptr<const GameObject> second, std::function<void()> handler){
	CollisionPair CP;
	CP.first = first;
	CP.second = second;
	CP.handler = handler;
	pairs.push_back(CP);
}

void CollisionSystem::removeObject(std::shared_ptr<const GameObject> GO){
	auto it = remove_if(pairs.begin(), pairs.end(), [GO](CollisionPair pair) -> bool { return pair.first == GO || pair.second == GO; });
	pairs.erase(it, pairs.end());
}


