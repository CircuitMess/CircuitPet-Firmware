#ifndef CIRCUITPET_FIRMWARE_GAMEOBJECT_H
#define CIRCUITPET_FIRMWARE_GAMEOBJECT_H

#include <memory>
#include "Rendering/RenderComponent.h"
#include "PixelDim.hpp"
#include "Collision/CollisionComponent.h"

class GameObject {
public:
	GameObject(std::unique_ptr<RenderComponent> rc, std::unique_ptr<CollisionComponent> cc);

	const PixelDim& getPos() const;
	void setPos(const PixelDim& pos);

	//TODO - razraditi ovo i implementirati
	void clone();

	std::shared_ptr<const RenderComponent> getRenderComponent() const;
	std::shared_ptr<const CollisionComponent> getCollisionComponent() const;

private:
	PixelDim pos;
	std::shared_ptr<RenderComponent> renderComponent;
	std::shared_ptr<CollisionComponent> collisionComponent;
};


#endif //CIRCUITPET_FIRMWARE_GAMEOBJECT_H
