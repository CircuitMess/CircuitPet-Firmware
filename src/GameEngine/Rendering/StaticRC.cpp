#include "StaticRC.h"

void StaticRC::push(Sprite* parent, PixelDim pos) const{
	if(!file) return;

	parent->drawIcon(file, pos.x, pos.y, dim.x, dim.y);
}

StaticRC::StaticRC(File file, const PixelDim& dim) : file(file), dim(dim){}
