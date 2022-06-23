#include "GameObject.h"

GameObject::GameObject(std::unique_ptr<RenderComponent> rc) : renderComponent(rc.release()){
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


