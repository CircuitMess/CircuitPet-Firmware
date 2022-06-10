
#include "OSSprite.h"
#include <SPIFFS.h>


OSSprite::OSSprite(Sprite* parent, uint8_t level) : sprite(parent, width, height){
	setLevel(level);
}

void OSSprite::setLevel(uint8_t level){
	String path = String("/OS/Level") + level + ".raw";
	fs::File osFile = SPIFFS.open(path);
	if(!osFile){printf("cant' open\n");}
	sprite.drawIcon(osFile, 0, 0, width, height);
}

void OSSprite::setPos(uint8_t x, uint8_t y){
	sprite.setPos(x,y);
}

void OSSprite::push(){
	sprite.push();
}
