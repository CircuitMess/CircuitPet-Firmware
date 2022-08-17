#ifndef CIRCUITPET_GAMESCROLLER_H
#define CIRCUITPET_GAMESCROLLER_H

#include <Arduino.h>
#include <CircuitOS.h>
#include <Loop/LoopListener.h>
#include <Util/Vector.h>
#include <functional>
#include "GameImage.h"
#include <math.h>

class Sprite;
class Launcher;



struct MenuItem {
	String text;
	GameImage image;
	GameImage imageLocked;
	uint8_t levelRequired;
	std::function<void()> primary;
	std::function<void()> secondary;
	bool loaded = false;

	MenuItem(String text, uint8_t levelRequired, const GameImage& image = GameImage(), const GameImage& imageLocked = GameImage(), std::function<void()> primary = {},
			 std::function<void()> secondary = {});

};

class Menu : public LoopListener {
public:
	Menu(Sprite* canvas);
	Menu(Sprite* canvas, std::vector<MenuItem>& items);
	~Menu();

	void setCanvas(Sprite* canvas);
	void setItems(std::vector<MenuItem>& items);

	void push();
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

	void setOffsetY(int16_t y);

	void shake();
	bool isShaking();

private:
	const uint8_t gutter = 25;
	const uint8_t width = 32;
	const uint8_t origin;
	const uint8_t originY = 75;
	int16_t offsetY = 0;
	const float speed = 250;
	fs::File borderFile;
	const char* borderPath = "/MenuIcons/Border.raw";

	Sprite* canvas;
	std::vector<MenuItem> items;

	uint selectedGame = 0;
	enum Direction { PREV, NEXT } direction;
	float delta = 0;
	bool queued = false;
	uint8_t multiplier = 1;

	//////////States
	enum State{neutral, moving, shaking};
	State state = neutral;

	//////////Shaking
	float peakAmplitude = 4.0f;
	float velocity = 30.0f;
	const float duration = 4*M_PI/velocity;
	float time = 0.0f;

	void selectNext();
	void selectPrev();

	GameImage* getCGame();
	GameImage* getLGame();
	GameImage* getRGame();
	GameImage* getLLGame();
	GameImage* getRRGame();
};


#endif //CIRCUITPET_GAMESCROLLER_H