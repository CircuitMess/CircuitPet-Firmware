
#include "BgSprite.h"
#include <SPIFFS.h>

BgSprite::BgSprite(Sprite* parent, uint8_t level) : sprite(parent, w, h){
	setLevel(level);
}

void BgSprite::setLevel(uint8_t level){
	String path = String("/Bg/Level") + level + ".raw";
	fs::File bgFile = SPIFFS.open(path);
	sprite.drawIcon(bgFile, 0, 0, width, height);
}

void BgSprite::push(){
	sprite.push();
}
