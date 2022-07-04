#include "CollisionSystem.h"

#include <utility>
#include "RectCC.h"
#include "CircleCC.h"

#undef abs

CollisionSystem::CollisionSystem(const Game* game) : GameSystem(game), Walls({
	 .top =  { nullptr, std::make_unique<RectCC>(glm::vec2{ 160, 100 }) },
	 .bot =  { nullptr, std::make_unique<RectCC>(glm::vec2{ 160, 100 }) },
	 .left =  { nullptr, std::make_unique<RectCC>(glm::vec2{ 100, 128 }) },
	 .right =  { nullptr, std::make_unique<RectCC>(glm::vec2{ 100, 128 }) }
}){
	Walls.top.setPos({ 0, -100 });
	Walls.bot.setPos({ 0, 128 });
	Walls.left.setPos({ -100, 0 });
	Walls.right.setPos({ 160, 0 });
}

void CollisionSystem::update(uint32_t deltaMicros){
	for(auto& pair : pairs){
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

		if(overlap && !pair.colliding){
			pair.handler();
		}

		pair.colliding = overlap;
	}
}

void CollisionSystem::addPair(const GameObject& first, const GameObject& second, std::function<void()> handler){
	if(&first == &second) return;
	if(!first.getCollisionComponent() || !second.getCollisionComponent()) return;

	pairs.push_back({ &first, &second, std::move(handler) });
}

void CollisionSystem::removePair(const GameObject& first, const GameObject& second){
	auto it = remove_if(pairs.begin(), pairs.end(), [first, second](const Pair& pair) -> bool {
		return (pair.first == &first && pair.second == &second) || (pair.first == &second && pair.second == &first);
	});

	pairs.erase(it, pairs.end());
}

void CollisionSystem::removeObject(const GameObject& GO){
	auto it = remove_if(pairs.begin(), pairs.end(), [GO](const Pair& pair) -> bool { return pair.first == &GO || pair.second == &GO; });
	pairs.erase(it, pairs.end());
}

void CollisionSystem::wallTop(const GameObject& obj, std::function<void()> handler){
	if(handler){
		addPair(obj, Walls.top, std::move(handler));
	}else{
		removePair(obj, Walls.top);
	}
}

void CollisionSystem::wallBot(const GameObject& obj, std::function<void()> handler){
	if(handler){
		addPair(obj, Walls.bot, std::move(handler));
	}else{
		removePair(obj, Walls.bot);
	}
}

void CollisionSystem::wallLeft(const GameObject& obj, std::function<void()> handler){
	if(handler){
		addPair(obj, Walls.left, std::move(handler));
	}else{
		removePair(obj, Walls.left);
	}
}

void CollisionSystem::wallRight(const GameObject& obj, std::function<void()> handler){
	if(handler){
		addPair(obj, Walls.right, std::move(handler));
	}else{
		removePair(obj, Walls.right);
	}
}

void CollisionSystem::wallsVertical(const GameObject& obj, std::function<void()> handler){
	wallLeft(obj, handler);
	wallRight(obj, handler);
}

void CollisionSystem::wallsHorizontal(const GameObject& obj, std::function<void()> handler){
	wallTop(obj, handler);
	wallBot(obj, handler);
}

void CollisionSystem::wallsAll(const GameObject& obj, std::function<void()> handler){
	wallLeft(obj, handler);
	wallRight(obj, handler);
	wallTop(obj, handler);
	wallBot(obj, handler);
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

	auto pos1 = circle1.getPos() + circle1.getCollisionComponent()->getCircle()->getOffset();
	auto pos2 = circle2.getPos() + circle2.getCollisionComponent()->getCircle()->getOffset();

	return glm::distance(pos1, pos2) <= r1 + r2;
}

bool CollisionSystem::rectCircle(const GameObject& rect, const GameObject& circle){
	// TODO: center offsetting

	auto cPos = circle.getPos() +  circle.getCollisionComponent()->getCircle()->getOffset();;
	auto r = circle.getCollisionComponent()->getCircle()->getRadius();
	auto rDim = rect.getCollisionComponent()->getRect()->getDim();
	auto rPos = rect.getPos() + 0.5f * rDim;

	// Thank you https://www.gamedevelopment.blog/collision-detection-circles-rectangles-and-polygons/
	auto distance = glm::abs(cPos - rPos);
	if (distance.x > (rDim.x/2 + r)) { return false; }
	if (distance.y > (rDim.y/2 + r)) { return false; }
	if (distance.x <= (rDim.x/2)) { return true; }
	if (distance.y <= (rDim.y/2)) { return true; }

	return glm::distance(distance, rDim * 0.5f) <= r;
}

void CollisionSystem::drawDebug(Sprite* canvas){
	std::set<const GameObject*> drawn;

	for(const auto& pair : pairs){
		Color c = pair.colliding ? TFT_GREEN : TFT_RED;

		const auto draw = [canvas, &drawn](Color c, const GameObject& obj){
			if(drawn.count(&obj) && c != TFT_GREEN) return;
			drawn.insert(&obj);

			auto col = obj.getCollisionComponent();

			if(col->getType() == CollisionType::Rect){
				canvas->drawRect(obj.getPos().x, obj.getPos().y, col->getRect()->getDim().x, col->getRect()->getDim().y, c);
			}else if(col->getType() == CollisionType::Circle){
				canvas->drawCircle(obj.getPos().x + obj.getCollisionComponent()->getCircle()->getOffset().x,
								   obj.getPos().y + obj.getCollisionComponent()->getCircle()->getOffset().y,
								   col->getCircle()->getRadius(), c);
			}
		};

		draw(c, *pair.first);
		draw(c, *pair.second);
	}
}
