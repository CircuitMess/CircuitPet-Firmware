
#include "OSSprite.h"
#include <SPIFFS.h>


OSSprite::OSSprite(Sprite* parent, uint8_t level) : sprite(parent, w, h){
	setLevel(level);
}

void OSSprite::setLevel(uint8_t level){
	char imgPath[50];
	sprintf(imgPath, "/Systems/%d.raw", level);
	fs::File osFile = SPIFFS.open(imgPath);
	if(!osFile){printf("cant' open\n");}
	sprite.drawIcon(osFile, 0, 0, w, h);
}

void OSSprite::setPos(uint8_t x, uint8_t y){
	sprite.setPos(x,y);
}

void OSSprite::push(){
	sprite.push();
}
