#include "CharacterSprite.h"
#include <sstream>
#include <SPIFFS.h>


const char* animNames[] = { "general", "scratch", "lookAround", "stretch", "wave", "dance", "knock" };

CharacterSprite::CharacterSprite(Sprite* parentSprite, uint8_t charLevel, bool rusty, Anim currentAnim) : parentSprite(parentSprite),
																												 charLevel(charLevel), rusty(rusty),
																												 currentAnim(currentAnim),
																												 sprite(std::make_unique<GIFAnimatedSprite>(parentSprite,getAnimFile(charLevel, rusty,currentAnim))){
	sprite->setLoopMode(LoopMode::INFINITE);
	sprite->setXY(30, 30);
}

void CharacterSprite::loop(uint micros){
	if(canChange){
		startNextAnim();
		canChange = false;
	}
}

void CharacterSprite::push(){
	sprite->push();
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

	sprite = std::make_unique<GIFAnimatedSprite>(parentSprite, getAnimFile(nextAnim->charLevel, nextAnim->rusty, nextAnim->anim));
	sprite->setLoopMode(LoopMode::INFINITE);
	sprite->setXY(30, 30);
	sprite->setLoopDoneCallback(nullptr);

	nextAnim = std::experimental::nullopt;
}

File CharacterSprite::getAnimFile(uint8_t charLevel, bool rusty, Anim anim){
	std::ostringstream path;
	path << "level" << (int)charLevel << "_rust" << (int)rusty << "_" << animNames[(uint8_t)anim] << ".gif";

	return SPIFFS.open(path.str().c_str());


}

void CharacterSprite::registerNextAnim(){
	nextAnim = CharacterAnim { charLevel, rusty, currentAnim };
	sprite->setLoopDoneCallback([this](){
		canChange = true;
	});
}
