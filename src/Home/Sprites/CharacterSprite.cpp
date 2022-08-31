#include "CharacterSprite.h"
#include <sstream>
#include <SPIFFS.h>

const char* animNames[] = { "general", "scratch", "look", "stretch", "wave", "dance", "knock" };
bool CharacterSprite::canChange = false;
CharacterSprite::CharacterSprite(Sprite* parentSprite, uint8_t charLevel, bool rusty, Anim currentAnim) : parentSprite(parentSprite),
																										  charLevel(charLevel), rusty(rusty),
																										  currentAnim(currentAnim),
																										  gif(std::make_unique<AnimatedSprite>(parentSprite, getAnimFile(charLevel, rusty, currentAnim))){
	gif->setLoop(true);
	gif->setXY(x, y);
	canChange = false;
	gif->setMaskingColor(TFT_TRANSPARENT);
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
		gif->start();
	}

	if(gif->checkFrame()){
		gif->nextFrame();
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
	gif = std::make_unique<AnimatedSprite>(parentSprite, getAnimFile(nextAnim->charLevel, nextAnim->rusty, nextAnim->anim));

	gif->setLoop(true);
	gif->setMaskingColor(TFT_TRANSPARENT);
	gif->setXY(x, y);
	gif->setLoopDoneCallback(nullptr);

	if(!firstPush){
		gif->start();
	}

	nextAnim = std::experimental::nullopt;
}

File CharacterSprite::getAnimFile(uint8_t charLevel, bool rusty, Anim anim){
	uint8_t level = getGIFLevel(charLevel);
	char path[50];
	if(rusty){
		sprintf(path, "/Home/rusty/%02d_%s.g565", level, animNames[(uint8_t)anim]);
	}else{
		sprintf(path, "/Home/%02d_%s.g565", level, animNames[(uint8_t)anim]);
	}
	return SPIFFS.open(path);
}

void CharacterSprite::registerNextAnim(){
	nextAnim = CharacterAnim { charLevel, rusty, currentAnim };
	gif->setLoopDoneCallback([](){
		CharacterSprite::canChange = true;
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

