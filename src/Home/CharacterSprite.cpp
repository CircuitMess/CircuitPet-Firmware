#include "CharacterSprite.h"
#include <sstream>
#include <SPIFFS.h>


const char* animNames[] = { "general", "scratch", "lookAround", "stretch", "wave", "dance", "knock" };

CharacterSprite::CharacterSprite(Sprite* parentSprite, uint8_t charLevel, uint8_t rustLevel, Anim currentAnim) : parentSprite(parentSprite),
																												 charLevel(charLevel), rusty(rustLevel),
																												 currentAnim(currentAnim),
																												 sprite(std::make_unique<GIFAnimatedSprite>(parentSprite,getAnimFile(charLevel, rustLevel,currentAnim))){
	sprite->setLoopMode(LoopMode::INFINITE);
	sprite->setXY(30, 30);
}

void CharacterSprite::loop(){
	sprite->push();
}

void CharacterSprite::setCharLevel(uint8_t charLevel){
	if(CharacterSprite::charLevel == charLevel) return;

	CharacterSprite::charLevel = charLevel;
	registerNextAnim();
}

void CharacterSprite::setRustLevel(bool rustLevel){
	if(CharacterSprite::rusty == rustLevel) return;

	CharacterSprite::rusty = rustLevel;
	registerNextAnim();
}

void CharacterSprite::setAnim(Anim anim){
	if(CharacterSprite::currentAnim == anim) return;

	CharacterSprite::currentAnim = anim;
	registerNextAnim();
}

void CharacterSprite::startNextAnim(){
	if(!nextAnim) return;

	sprite = std::make_unique<GIFAnimatedSprite>(parentSprite, getAnimFile(nextAnim->charLevel, nextAnim->rustLevel, nextAnim->anim));
	sprite->setLoopMode(LoopMode::INFINITE);
	sprite->setXY(30, 30);
	sprite->setLoopDoneCallback(nullptr);

	nextAnim = std::experimental::nullopt;
}

File CharacterSprite::getAnimFile(uint8_t charLevel, uint8_t rustLevel, Anim anim){
	std::ostringstream path;
	path << "level" << (int)charLevel << "_rust" << (int)rustLevel << "_" << animNames[(uint8_t)anim] << ".gif";

	return SPIFFS.open(path.str().c_str());


}

void CharacterSprite::registerNextAnim(){
	nextAnim = CharacterAnim { charLevel, rusty, currentAnim };
	sprite->setLoopDoneCallback([this](){
		startNextAnim();
	});
}
