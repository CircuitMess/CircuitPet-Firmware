#include "CharacterSprite.h"
#include <sstream>
#include <SPIFFS.h>

const char* animNames[] = { "general", "scratch", "look", "stretch", "wave", "dance", "knock" };

CharacterSprite::CharacterSprite(Sprite* parentSprite, uint8_t charLevel, bool rusty, Anim currentAnim) : parentSprite(parentSprite),
																										  charLevel(charLevel), rusty(rusty),
																										  currentAnim(currentAnim),
																										  gif(std::make_unique<GIFAnimatedSprite>(parentSprite, getAnimFile(charLevel, rusty, currentAnim))){
	gif->setLoopMode(GIF::Infinite);
	gif->setXY(x, y);
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
		// gif->nextFrame();
		gif->start();
	}
	gif->push();
}

void CharacterSprite::setCharLevel(uint8_t charLevel){
	if(CharacterSprite::charLevel == charLevel) return;

	uint8_t curr = CharacterSprite::charLevel;
	CharacterSprite::charLevel = charLevel;

	if(getGIFLevel(curr) == getGIFLevel(charLevel)) return;

	registerNextAnim();
	startNextAnim();
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
	gif.reset();
	gif = std::make_unique<GIFAnimatedSprite>(parentSprite, getAnimFile(nextAnim->charLevel, nextAnim->rusty, nextAnim->anim));

	gif->setLoopMode(GIF::Infinite);
	gif->setXY(x, y);
	gif->setLoopDoneCallback(nullptr);

	if(!firstPush){
		// gif->nextFrame();
		gif->start();
	}

	nextAnim = std::experimental::nullopt;
}

File CharacterSprite::getAnimFile(uint8_t charLevel, bool rusty, Anim anim){
	uint8_t level = getGIFLevel(charLevel);
	char path[50];
	if(rusty){
		sprintf(path, "/Home/rusty/%02d_%s.gif", level, animNames[(uint8_t)anim]);
	}else{
		sprintf(path, "/Home/%02d_%s.gif", level, animNames[(uint8_t)anim]);
	}
	return SPIFFS.open(path);
}

void CharacterSprite::registerNextAnim(){
	nextAnim = CharacterAnim { charLevel, rusty, currentAnim };
	gif->setLoopDoneCallback([this](uint32_t){
		canChange = true;
	});
}

void CharacterSprite::setPos(int16_t x, int16_t y){
	this->x = x;
	this->y = y;
	gif->setXY(x, y);
}

uint8_t CharacterSprite::getGIFLevel(uint8_t level){
	uint8_t nextLevel;
	if(level <= 3){
		nextLevel = 1;
	}else if(level <= 5){
		nextLevel = 4;
	}else{
		nextLevel = 6;
	}
	return nextLevel;
}
