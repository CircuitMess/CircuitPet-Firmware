#ifndef CIRCUITPET_FIRMWARE_GAMEOBJECT_H
#define CIRCUITPET_FIRMWARE_GAMEOBJECT_H

#include <memory>
#include "Rendering/RenderComponent.h"
#include "PixelDim.hpp"
#include "Collision/CollisionComponent.h"

class GameObject {
public:
	GameObject(std::unique_ptr<RenderComponent> rc, std::unique_ptr<CollisionComponent> cc);

	glm::vec2 getPos() const;
	void setPos(glm::vec2 pos);

	//TODO - razraditi ovo i implementirati
	void clone();

	std::shared_ptr<RenderComponent> getRenderComponent() const;
	std::shared_ptr<const CollisionComponent> getCollisionComponent() const;

private:
	glm::vec2 pos;
	std::shared_ptr<RenderComponent> renderComponent;
	std::shared_ptr<CollisionComponent> collisionComponent;

};


#endif //CIRCUITPET_FIRMWARE_GAMEOBJECT_H
