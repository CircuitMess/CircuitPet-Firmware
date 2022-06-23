#include "GameObject.h"

GameObject::GameObject(std::unique_ptr<RenderComponent> rc, std::unique_ptr<CollisionComponent> cc) : renderComponent(rc.release()),
																									  collisionComponent(cc.release()){
}

const PixelDim& GameObject::getPos(){
	return pos;
}

void GameObject::setPos(const PixelDim& pos){
	GameObject::pos = pos;
}

std::shared_ptr<const RenderComponent> GameObject::getRenderComponent() const{
	return renderComponent;
}

std::shared_ptr<const CollisionComponent> GameObject::getCollisionComponent() const{
	return collisionComponent;
}


