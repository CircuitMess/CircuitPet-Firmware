
#include "OSSprite.h"
#include <SPIFFS.h>


OSSprite::OSSprite(Sprite* parent, uint8_t level) : sprite(parent, osWidth+logoWidth+1, logoHeight){
	setLevel(level);
}

void OSSprite::setLevel(uint8_t level){
	String path = String("/OS/Level") + level + ".raw";
	fs::File osFile = SPIFFS.open(path);
	fs::File logoFile = SPIFFS.open("/OS/Logo.raw");

	sprite.clear(TFT_TRANSPARENT);
	sprite.drawIcon(logoFile, 0, 0, logoWidth, logoHeight);
	sprite.drawIcon(osFile, logoWidth + 1, 1, osWidth, osHeight);
}

void OSSprite::setPos(uint8_t x, uint8_t y){
	sprite.setPos(x,y);
}

void OSSprite::push(){
	sprite.push();
}
