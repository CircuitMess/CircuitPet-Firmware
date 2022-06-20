#include "GameObject.h"

GameObject::GameObject(RenderComponent* rc) : renderComponent(rc){
}

const PixelDim& GameObject::getPos(){
	return pos;
}

void GameObject::setPos(const PixelDim& pos){
	GameObject::pos = pos;
}

std::shared_ptr<const RenderComponent> GameObject::getRenderComponent(){
	return renderComponent;
}


