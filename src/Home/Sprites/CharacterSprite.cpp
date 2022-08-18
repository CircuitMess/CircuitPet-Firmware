#include "CharacterSprite.h"
#include <sstream>
#include <SPIFFS.h>
#include <FS/RamFile.h>
#include <esp_heap_caps.h>

const char* animNames[] = { "general", "scratch", "look", "stretch", "wave", "dance", "knock" };

CharacterSprite::CharacterSprite(Sprite* parentSprite, uint8_t charLevel, bool rusty, Anim currentAnim) : parentSprite(parentSprite),
																										  charLevel(charLevel), rusty(rusty),
																										  currentAnim(currentAnim){
	preloadAnims();
	gif = std::make_unique<GIFAnimatedSprite>(parentSprite, animMap[currentAnim]);
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
		gif->start();
	}
	gif->push();
}

void CharacterSprite::setCharLevel(uint8_t charLevel){
	if(CharacterSprite::charLevel == charLevel) return;

	uint8_t curr = CharacterSprite::charLevel;
	CharacterSprite::charLevel = charLevel;
//	Serial.printf("curr: %d, charLevel: %d\n", curr, charLevel);
	if(getGIFLevel(curr) != getGIFLevel(charLevel)){
		preloadAnims();
	}

	registerNextAnim();
	startNextAnim();
}

void CharacterSprite::setRusty(bool rusty){
	if(CharacterSprite::rusty == rusty) return;

	CharacterSprite::rusty = rusty;
	preloadAnims();
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
	animMap[nextAnim->anim].seek(0);
	gif = std::make_unique<GIFAnimatedSprite>(parentSprite, animMap[nextAnim->anim]);

	gif->setLoopMode(GIF::Infinite);
	gif->setXY(x, y);
	gif->setLoopDoneCallback(nullptr);

	if(!firstPush){
		gif->start();
	}

	nextAnim = std::experimental::nullopt;
}

File CharacterSprite::getAnimFile(uint8_t charLevel, bool rusty, Anim anim){
	uint8_t nextLevel = getGIFLevel(charLevel);

	char path[50];
	if(rusty){
		sprintf(path, "/Home/rusty/%02d_%s.gif", nextLevel, animNames[(uint8_t)anim]);
	}else{
		sprintf(path, "/Home/%02d_%s.gif", nextLevel, animNames[(uint8_t)anim]);
	}
//	Serial.println(path);
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

void CharacterSprite::preloadAnims(){
	for (uint8_t i = 0; i < (uint8_t)Anim::Count; i++){
		animMap[(Anim)i].close();
	}


	for (uint8_t i = 0; i < (uint8_t)Anim::Count; i++) {
//		animMap[(Anim)i].close();
//		Serial.printf("free heap: %d, largest chunk: %zu\n", ESP.getFreeHeap(), heap_caps_get_largest_free_block(MALLOC_CAP_8BIT | MALLOC_CAP_INTERNAL));
		File s = getAnimFile(charLevel, rusty, (Anim)i);
		File f = RamFile::open(s);
		animMap[(Anim)i] = f;
	}
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
