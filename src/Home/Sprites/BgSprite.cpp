
#include "BgSprite.h"
#include <SPIFFS.h>

BgSprite::BgSprite(Sprite* parent, uint8_t level) : sprite(parent, width, height){
	setLevel(level);
}

void BgSprite::setLevel(uint8_t level){
	char path[20];
	sprintf(path, "/Bg/Level%d.raw", level);
	fs::File bgFile = SPIFFS.open(path);
	sprite.drawIcon(bgFile, 0, 0, width, height);
}

void BgSprite::push(){
	sprite.push();
}
