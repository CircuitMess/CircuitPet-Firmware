
#include "SpriteRC.h"

SpriteRC::SpriteRC(PixelDim dim) : sprite(std::make_shared<Sprite>((Sprite*) nullptr, dim.x, dim.y)){
	static_cast<TFT_eSprite*>(sprite.get())->setSwapBytes(false); //has to be called since it's not inherited from baseSprite or any other sprite
}

std::shared_ptr<Sprite> SpriteRC::getSprite() const{
	return sprite;
}

void SpriteRC::push(Sprite* parent, PixelDim pos) const{
	sprite->push(parent, pos.x, pos.y);
}
