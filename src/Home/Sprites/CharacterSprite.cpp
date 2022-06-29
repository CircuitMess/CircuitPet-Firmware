#include "CharacterSprite.h"
#include <sstream>
#include <SPIFFS.h>


const char* animNames[] = { "general", "scratch", "lookAround", "stretch", "wave", "dance", "knock" };

CharacterSprite::CharacterSprite(Sprite* parentSprite, uint8_t charLevel, bool rusty, Anim currentAnim) : parentSprite(parentSprite),
																												 charLevel(charLevel), rusty(rusty),
																												 currentAnim(currentAnim),
																												 sprite(parentSprite,getAnimFile(charLevel, rusty,currentAnim)){
	sprite.setLoopMode(GIF::Infinite);
	sprite.setXY(x, y);
}

void CharacterSprite::loop(uint micros){
	if(canChange){
		startNextAnim();
		canChange = false;
	}
}

void CharacterSprite::push(){
	if(firstPush){
		firstPush = false;
		sprite.start();
	}
	sprite.push();
}

void CharacterSprite::setCharLevel(uint8_t charLevel){
	if(CharacterSprite::charLevel == charLevel) return;

	CharacterSprite::charLevel = charLevel;
	registerNextAnim();
}

void CharacterSprite::setRusty(bool rusty){
	if(CharacterSprite::rusty == rusty) return;

	CharacterSprite::rusty = rusty;
	registerNextAnim();
}

void CharacterSprite::setAnim(Anim anim){
	if(CharacterSprite::currentAnim == anim) return;

	CharacterSprite::currentAnim = anim;
	registerNextAnim();
}

void CharacterSprite::startNextAnim(){
	if(!nextAnim) return;

	sprite = GIFAnimatedSprite(parentSprite, getAnimFile(nextAnim->charLevel, nextAnim->rusty, nextAnim->anim));

	sprite.setLoopMode(GIF::Infinite);
	sprite.setXY(x, y);
	sprite.setLoopDoneCallback(nullptr);

	if(!firstPush){
		sprite.start();
	}

	nextAnim = std::experimental::nullopt;
}

File CharacterSprite::getAnimFile(uint8_t charLevel, bool rusty, Anim anim){
	std::ostringstream path;
	path << "level" << (int)charLevel << "_rust" << (int)rusty << "_" << animNames[(uint8_t)anim] << ".gif";

	return SPIFFS.open(path.str().c_str());
}

void CharacterSprite::registerNextAnim(){
	nextAnim = CharacterAnim { charLevel, rusty, currentAnim };
	sprite.setLoopDoneCallback([this](uint32_t){
		canChange = true;
	});
}

void CharacterSprite::setPos(int16_t x, int16_t y){
	this->x = x;
	this->y = y;
	sprite.setXY(x, y);
}
