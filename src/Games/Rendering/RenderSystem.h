#ifndef CIRCUITPET_FIRMWARE_RENDERSYSTEM_H
#define CIRCUITPET_FIRMWARE_RENDERSYSTEM_H

#include <Display/Sprite.h>
#include "../GameSystem.h"

class RenderSystem : public GameSystem {
public:
	RenderSystem(const std::vector<GameObject>& objects, Sprite* canvas);

	void render(); //iterate objects by layers and push them to the canvas
private:
	Sprite* canvas;
};


#endif //CIRCUITPET_FIRMWARE_RENDERSYSTEM_H
