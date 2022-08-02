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

class DuckScreen : public LoopListener, public State, private InputListener {
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
	CharacterSprite characterSprite;
	std::unique_ptr<StatsSprite> statsSprite;
	Menu menu;
	MenuHider hider;

	uint8_t selection = 0;

	std::vector<MenuItem> menuItems;

	constexpr static uint8_t osX = 125;
	constexpr static uint8_t osY = 1;

	constexpr static uint8_t characterX = 53;
	constexpr static uint8_t characterY = 29;

	constexpr static uint8_t statsX = 1;
	constexpr static uint8_t statsY = 0;

	constexpr static uint8_t menuY = 64;

	void buttonPressed(uint i) override;
};


#endif //CIRCUITPET_FIRMWARE_DUCKSCREEN_H
