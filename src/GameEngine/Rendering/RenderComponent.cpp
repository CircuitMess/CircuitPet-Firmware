#include "RenderComponent.h"

int8_t RenderComponent::getLayer() const{
	return layer;
}

void RenderComponent::setLayer(int8_t layer){
	RenderComponent::layer = layer;
}
