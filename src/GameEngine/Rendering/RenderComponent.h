#ifndef CIRCUITPET_FIRMWARE_RENDERCOMPONENT_H
#define CIRCUITPET_FIRMWARE_RENDERCOMPONENT_H

#include <Display/Sprite.h>
#include "../PixelDim.hpp"

class RenderSystem;

class RenderComponent {
	friend RenderSystem;

public:
	uint8_t getLayer() const;
	void setLayer(uint8_t layer);

protected:
	virtual void push(Sprite* parent, PixelDim pos) const = 0;

private:
	uint8_t layer = 0;
};


#endif //CIRCUITPET_FIRMWARE_RENDERCOMPONENT_H
