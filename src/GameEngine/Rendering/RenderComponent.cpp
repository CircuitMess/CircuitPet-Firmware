#include "RenderComponent.h"

uint8_t RenderComponent::getLayer() const{
	return layer;
}

void RenderComponent::setLayer(uint8_t layer){
	RenderComponent::layer = layer;
}
