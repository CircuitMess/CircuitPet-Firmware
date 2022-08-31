#include "BgSprite.h"
#include <FS/CompressedFile.h>
#include <SPIFFS.h>

BgSprite::BgSprite(Sprite* parent, uint8_t level) : sprite(parent, width, height){
	setLevel(level);
}

void BgSprite::setLevel(uint8_t level){
	char path[20];
	sprintf(path, "/Bg/Level%d.hs", level);
	fs::File bgFile = CompressedFile::open(SPIFFS.open(path), 8, 4);
	sprite.drawIcon(bgFile, 0, 0, width, height);
}

void BgSprite::push(){
	sprite.push();
}
