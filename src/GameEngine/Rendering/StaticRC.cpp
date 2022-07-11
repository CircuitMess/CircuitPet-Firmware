#include "StaticRC.h"

#include <utility>

StaticRC::StaticRC(File file, PixelDim dim) : file(std::move(file)), dim(dim){}

void StaticRC::push(Sprite* parent, PixelDim pos) const{
	if(!file){
		ESP_LOGE("Render", "StaticRC: Pushing closed file");
		return;
	}
	
	parent->drawIcon(file, pos.x, pos.y, dim.x, dim.y);
}

void StaticRC::setFile(File file){
	StaticRC::file = file;
}

void StaticRC::setFile(File file, PixelDim dim){
	StaticRC::file = file;
	StaticRC::dim = dim;
}

