#ifndef CIRCUITPET_FIRMWARE_DUCKSCREEN_H
#define CIRCUITPET_FIRMWARE_DUCKSCREEN_H

#include "../State.h"
#include <Loop/LoopListener.h>
#include <Input/InputListener.h>
#include "Sprites/BgSprite.h"
#include "Sprites/OSSprite.h"
#include "Sprites/CharacterSprite.h"
#include "Sprites/StatsSprite.h"
#include "Menu/Menu.h"
#include "Menu/MenuHider.h"
#include "../Stats/StatsChangedListener.h"

class DuckScreen : public LoopListener, public State, private InputListener, private StatsChangedListener {
public:
	DuckScreen(Sprite* base);
	void loop(uint micros) override;

protected:
	void onStart() override;
	void onStop() override;

private:
	Sprite* base;

	std::unique_ptr<BgSprite> bgSprite;
	std::unique_ptr<OSSprite> osSprite;
	std::unique_ptr<CharacterSprite> characterSprite;

	std::unique_ptr<StatsSprite> statsSprite;
	void statsChanged(const Stats& stats, bool leveledUp) override;
	constexpr static float easeTime = 0.6f;
	float easeTimer = 0;
	Stats currentStats;
	Stats targetStats;
	Stats prevStats;

	bool dead = false;
	constexpr static uint8_t rustThreshold = 25;

	Menu menu;
	MenuHider hider;

	uint32_t randCounter = 0;
	uint32_t randInterval = 0; //between 2s and 6s
	bool specialAnimPlaying = false;

	uint8_t selection = 0;

	std::vector<MenuItem> menuItems;

	constexpr static uint8_t osX = 125;
	constexpr static uint8_t osY = 1;

	constexpr static uint8_t characterX = 50;
	constexpr static uint8_t characterY = 36;

	constexpr static uint8_t statsX = 1;
	constexpr static uint8_t statsY = 0;

	void buttonPressed(uint i) override;

	enum LevelUpState { None, FadeIn, Image, FadeOut } luState = None;
	File luFile;
	uint32_t luMicros = 0;
	bool luApplied = false;
};


#endif //CIRCUITPET_FIRMWARE_DUCKSCREEN_H
