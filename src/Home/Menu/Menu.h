#ifndef BYTEBOI_GAMESCROLLER_H
#define BYTEBOI_GAMESCROLLER_H

#include <Arduino.h>
#include <CircuitOS.h>
#include <Loop/LoopListener.h>
#include <Util/Vector.h>
#include <functional>
#include "GameImage.h"

class Sprite;
class Launcher;



struct MenuItem {
	String text;
	GameImage image;
	std::function<void()> primary;
	std::function<void()> secondary;
	bool loaded = false;

	MenuItem(String text, const GameImage& image = GameImage(), std::function<void()> primary = {}, std::function<void()> secondary = {});
};

class Menu : public LoopListener {
public:
	Menu(Sprite* canvas, std::vector<MenuItem>& items);
	~Menu();
	void setCanvas(Sprite* canvas);

	void draw();
	void loop(uint micros);

	uint8_t prev();
	uint8_t next();

	bool scrolling();

	void splash(float f);
	void load(float f);
	void finish(float f);
	void reset();
	void repos();
	uint getSelectedIndex() const;

private:
	const uint8_t gutter = 25;
	const uint8_t width = 64;
	const uint8_t origin;
	const uint8_t originY = 35;
	const float speed = 250;

	Sprite* canvas;
	std::vector<MenuItem>& items;

	uint selectedGame = 0;
	enum Direction { PREV, NEXT } direction;
	float delta = 0;
	bool queued = false;
	uint8_t multiplier = 1;

	void selectNext();
	void selectPrev();

	GameImage* getCGame();
	GameImage* getLGame();
	GameImage* getRGame();
	GameImage* getLLGame();
	GameImage* getRRGame();
};


#endif //BYTEBOI_GAMESCROLLER_H