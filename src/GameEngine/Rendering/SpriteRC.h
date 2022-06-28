
#ifndef CIRCUITPET_FIRMWARE_SPRITERC_H
#define CIRCUITPET_FIRMWARE_SPRITERC_H


#include <memory>
#include <Display/Sprite.h>
#include "../PixelDim.hpp"
#include "RenderComponent.h"

class SpriteRC : public RenderComponent {
public:
	SpriteRC(PixelDim dim);
	std::shared_ptr<Sprite> getSprite();
protected:
	void push(Sprite* parent, PixelDim pos) const override;
private:
	std::shared_ptr<Sprite> sprite;
};


#endif //CIRCUITPET_FIRMWARE_SPRITERC_H
