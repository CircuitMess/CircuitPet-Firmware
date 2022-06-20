#ifndef CIRCUITPET_FIRMWARE_GAMEOBJECT_H
#define CIRCUITPET_FIRMWARE_GAMEOBJECT_H

#include <memory>
#include "Rendering/RenderComponent.h"
#include "PixelDim.hpp"

class GameObject {
public:
	GameObject(std::unique_ptr<RenderComponent> rc);

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
