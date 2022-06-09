
#include "BgSprite.h"
#include <SPIFFS.h>

BgSprite::BgSprite(Sprite* parent, uint8_t level) : sprite(parent, w, h){
	setLevel(level);
}

void BgSprite::setLevel(uint8_t level){
	char imgPath[50];
	sprintf(imgPath, "/Backgrounds/%d.raw", level);
	fs::File bgFile = SPIFFS.open(imgPath);
	sprite.drawIcon(bgFile, 0, 0, w, h);
}

void BgSprite::push(){
	sprite.push();
}
