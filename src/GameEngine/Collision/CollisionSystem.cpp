#include "CollisionSystem.h"
#include "SquareCC.h"

void CollisionSystem::collide(){
	for(const auto& pair : pairs){
		bool overlap = false;

		auto type1 = pair.first->getCollisionComponent()->getType();
		auto type2 = pair.second->getCollisionComponent()->getType();

		if(type1 == CollisionType::Square && type2 == CollisionType::Square){
			overlap = rectRect(*pair.first, *pair.second);
		}else if(type1 == CollisionType::Square && type2 == CollisionType::Circle){
			overlap = rectCircle(*pair.first, *pair.second);
		}else if(type1 == CollisionType::Circle && type2 == CollisionType::Square){
			overlap = rectCircle(*pair.second, *pair.first);
		}else if(type1 == CollisionType::Circle && type2 == CollisionType::Circle){
			overlap = circleCircle(*pair.first, *pair.second);
		}

		if(overlap){
			pair.handler();
		}
	}
}

void CollisionSystem::addPair(std::shared_ptr<const GameObject> first, std::shared_ptr<const GameObject> second, std::function<void()> handler){
	if(first == second) return;
	if(!first->getCollisionComponent() || !second->getCollisionComponent() || !handler) return;

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

bool CollisionSystem::rectRect(const GameObject square1, const GameObject square2){
	auto collision1 = square1.getCollisionComponent()->getSquare();
	auto collision2 = square2.getCollisionComponent()->getSquare();

	int16_t right1 = square1.getPos().x + collision1->getDim().x;
	int16_t right2 = square2.getPos().x + collision2->getDim().x;
	int16_t bottom1 = square1.getPos().y + collision1->getDim().y;
	int16_t bottom2 = square2.getPos().y + collision2->getDim().y;

	//taken from https://stackoverflow.com/questions/306316/determine-if-two-rectangles-overlap-each-other
	//changed < > to <= >= in case of touching, not overlapping
	return (square1.getPos().x <= right2 && right1 >= square2.getPos().x &&
			square1.getPos().y >= bottom2 && bottom1 <= square2.getPos().y);
}

bool CollisionSystem::circleCircle(const GameObject circle1, const GameObject circle2){
	return false;
}

bool CollisionSystem::rectCircle(const GameObject rect, const GameObject circle){
	return false;
}
