
#include "CollisionComponent.h"
#include "CircleCC.h"
#include "SquareCC.h"

CollisionComponent::CollisionComponent(CollisionType type) : type(type){

}

CollisionType CollisionComponent::getType() const{
	return type;
}

const CircleCC* CollisionComponent::getCircle() const{
	if(this->type != CollisionType::Circle) return nullptr;
	return reinterpret_cast<const CircleCC*>(this);
}

const SquareCC* CollisionComponent::getSquare() const{
	if(this->type != CollisionType::Square) return nullptr;
	return reinterpret_cast<const SquareCC*>(this);
}
