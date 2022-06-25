#include "CollisionSystem.h"

#include <utility>
#include "RectCC.h"
#include "CircleCC.h"

void CollisionSystem::collide(){
	for(const auto& pair : pairs){
		bool overlap = false;

		auto type1 = pair.first->getCollisionComponent()->getType();
		auto type2 = pair.second->getCollisionComponent()->getType();

		if(type1 == CollisionType::Rect && type2 == CollisionType::Rect){
			overlap = rectRect(*pair.first, *pair.second);
		}else if(type1 == CollisionType::Rect && type2 == CollisionType::Circle){
			overlap = rectCircle(*pair.first, *pair.second);
		}else if(type1 == CollisionType::Circle && type2 == CollisionType::Rect){
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

bool CollisionSystem::rectRect(const GameObject& square1, const GameObject& square2){
	auto pos1 = square1.getPos() - 0.5f;
	auto pos2 = square2.getPos() - 0.5f;
	auto dim1 = square1.getCollisionComponent()->getRect()->getDim() + 1.0f;
	auto dim2 = square2.getCollisionComponent()->getRect()->getDim() + 1.0f;

	return (pos1.x <= pos2.x + dim2.x && pos1.x + dim1.x >= pos2.x) &&
		   (pos1.y <= pos2.y + dim2.y && pos1.y + dim1.y >= pos2.y);
}

bool CollisionSystem::circleCircle(const GameObject& circle1, const GameObject& circle2){
	// TODO: center offsetting

	auto r1 = circle1.getCollisionComponent()->getCircle()->getRadius();
	auto r2 = circle2.getCollisionComponent()->getCircle()->getRadius();

	auto pos1 = circle1.getPos();
	auto pos2 = circle2.getPos();

	return glm::distance(pos1, pos2) <= r1 + r2;
}

bool CollisionSystem::rectCircle(const GameObject& rect, const GameObject& circle){
	// TODO: center offsetting

	auto cPos = circle.getPos();
	auto r = circle.getCollisionComponent()->getCircle()->getRadius();
	auto rPos = rect.getPos();
	auto rDim = rect.getCollisionComponent()->getRect()->getDim();

	// Thank you https://www.gamedevelopment.blog/collision-detection-circles-rectangles-and-polygons/
	auto distance = glm::abs(cPos - rPos);
	if (distance.x > (rDim.x/2 + r)) { return false; }
	if (distance.y > (rDim.y/2 + r)) { return false; }
	if (distance.x <= (rDim.x/2)) { return true; }
	if (distance.y <= (rDim.y/2)) { return true; }
	int16_t cDist_sq = pow((distance.x - rDim.x/2), 2) + pow((distance.y - rDim.y/2), 2);

	return glm::distance(distance, rDim * 0.5f) <= r;
}
