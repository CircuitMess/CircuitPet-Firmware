#ifndef CIRCUITPET_FIRMWARE_GAMEOBJECT_H
#define CIRCUITPET_FIRMWARE_GAMEOBJECT_H

#include <memory>
#include "vec2.hpp"
#include "RenderComponent.h"

class GameObject {
public:
	GameObject(RenderComponent* rc);

	const PixelDim& getPos();
	void setPos(const PixelDim& pos);

	//TODO - razraditi ovo i implementirati
	void clone();

	std::shared_ptr<const RenderComponent> getRenderComponent();

private:
	PixelDim pos;
	std::shared_ptr<RenderComponent> renderComponent;

};


#endif //CIRCUITPET_FIRMWARE_GAMEOBJECT_H
