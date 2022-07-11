#ifndef CIRCUITPET_FIRMWARE_GAME5_H
#define CIRCUITPET_FIRMWARE_GAME5_H

#include <Arduino.h>
#include <Input/InputListener.h>
#include "../GameEngine/Game.h"
#include "../GameEngine/Rendering/StaticRC.h"
#include "Chatter.h"
#include <deque>

class Game5 : public Game, private InputListener {

	typedef std::shared_ptr<GameObject> ObjPtr;

public:
	Game5();
protected:
	void onStart() override;
	void onStop() override;
	void onLoad() override;
	void onLoop(float deltaTime) override;
	void onRender(Sprite* canvas) override;


private:
	GameObject bottomWall;
	ObjPtr bars[3];
	ObjPtr circles[3];

	constexpr static uint16_t barsX[3] = {8, 26, 44};
	constexpr static const char* barsIcons[3] = {"/BarP.raw", "/BarY.raw", "/BarB.raw"};

	bool circlesPressed[3] = {false, false, false};
	constexpr static uint16_t circlesY = 109;
	constexpr static uint16_t circlesX[3] = {6, 24, 42};
	constexpr static const char* circlesIcons[3] = {"/circ_p1.raw", "/circ_y1.raw", "/circ_b1.raw"};
	constexpr static const char* circlesIconsPressed[3] = {"/circ_p2.raw", "/circ_y2.raw", "/circ_b2.raw"};


	std::deque<ObjPtr> notes[3];
	constexpr static const char* notesIcons[3] = {"/note1.raw", "/note2.raw", "/note3.raw"};
	void updateNotes(float delta);
	void createNote(uint8_t track);
	float notesSpeed = 50.0f;


	void buttonPressed(uint i) override;
	void buttonReleased(uint i) override;

};


#endif //CIRCUITPET_FIRMWARE_GAME5_H
