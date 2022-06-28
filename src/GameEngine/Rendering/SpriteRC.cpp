
#include "SpriteRC.h"

SpriteRC::SpriteRC(PixelDim dim){
	std::make_shared<Sprite>((Sprite*) nullptr, dim.x, dim.y);
}

std::shared_ptr<Sprite> SpriteRC::getSprite(){
	return sprite;
}

void SpriteRC::push(Sprite* parent, PixelDim pos) const{
	sprite->push(parent, pos.x, pos.y);
}
