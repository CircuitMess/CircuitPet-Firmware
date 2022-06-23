
#include "CollisionComponent.h"
#include "CircleCC.h"
#include "SquareCC.h"

CollisionComponent::CollisionComponent(Type type) : type(type){

}

CollisionComponent::Type CollisionComponent::getType(){
	return type;
}

CircleCC* CollisionComponent::getCircle(){
	if(this->type != Circle) return nullptr;
	return reinterpret_cast<CircleCC*>(this);
}

SquareCC* CollisionComponent::getSquare(){
	if(this->type != Square) return nullptr;
	return reinterpret_cast<SquareCC*>(this);
}
