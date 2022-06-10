#ifndef CIRCUITPET_FIRMWARE_CHARACTERSPRITE_H
#define CIRCUITPET_FIRMWARE_CHARACTERSPRITE_H

#include <Display/GIFAnimatedSprite.h>
#include <experimental/optional>

enum class Anim : uint8_t{
	general, scratch, lookAround, stretch, wave, dance, knock
};

struct CharacterAnim{
	uint8_t charLevel;
	uint8_t rustLevel;
	Anim anim;
};

class CharacterSprite {
public:
	CharacterSprite(Sprite* parentSprite, uint8_t charLevel, uint8_t rustLevel, Anim currentAnim);
	void loop();

	void setCharLevel(uint8_t charLevel);
	void setRustLevel(bool rustLevel);

	void setAnim(Anim anim);

private:
	Sprite* parentSprite;
	std::unique_ptr<GIFAnimatedSprite> sprite;

	uint8_t charLevel; //0-3
	bool rusty; //0-1
	Anim currentAnim;

	std::experimental::optional<CharacterAnim> nextAnim;

	void registerNextAnim(); //queues up nextAnim to be started when current anim ends
	void startNextAnim(); //starts queued nextAnim, then clears it
	File getAnimFile(uint8_t charLevel, uint8_t rustLevel, Anim anim);

};


#endif //CIRCUITPET_FIRMWARE_CHARACTERSPRITE_H
