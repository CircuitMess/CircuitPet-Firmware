#ifndef CIRCUITPET_FIRMWARE_CHARACTERSPRITE_H
#define CIRCUITPET_FIRMWARE_CHARACTERSPRITE_H

#include <Display/GIFAnimatedSprite.h>
#include <experimental/optional>

enum class Anim : uint8_t{
	General, Scratch, LookAround, Stretch, Wave, Dance, Knock
};

struct CharacterAnim{
	uint8_t charLevel;
	bool rusty;
	Anim anim;
};

class CharacterSprite : public LoopListener {
public:
	CharacterSprite(Sprite* parentSprite, uint8_t charLevel, bool rusty, Anim currentAnim);
	void loop(uint micros) override;
	void push();

	void setCharLevel(uint8_t charLevel);
	void setRusty(bool rusty);

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
	File getAnimFile(uint8_t charLevel, bool rustLevel, Anim anim);

	constexpr static uint16_t x = 30;
	constexpr static uint16_t y = 30;

	bool canChange = false;
};


#endif //CIRCUITPET_FIRMWARE_CHARACTERSPRITE_H
