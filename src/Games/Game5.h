#ifndef CIRCUITPET_FIRMWARE_GAME5_H
#define CIRCUITPET_FIRMWARE_GAME5_H

#include <Arduino.h>
#include <Input/InputListener.h>
#include "../GameEngine/Game.h"
#include "Chatter.h"
#include "../GameEngine/Rendering/AnimRC.h"
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
	enum {
		Running, DoneAnim, DonePause
	} state = Running;

	GameObject bottomWall;
	ObjPtr scoreBar;
	std::shared_ptr<Sprite> scoreBarSprite;
	ObjPtr hearts[3];

	ObjPtr scoreStar;
	ObjPtr bars[3];
	ObjPtr circles[3];
	ObjPtr duck;
	std::shared_ptr<AnimRC> duckRC;

	void gameDone(bool success);
	constexpr static float gameDonePause = 1.5f;
	float gameDoneTimer = 0;

	uint8_t life = 3;
	uint32_t score = 0;
	constexpr static uint32_t perfectBonus = 200;
	constexpr static uint32_t notePoints = 100;
	constexpr static uint32_t goal = 15 * (perfectBonus + notePoints);
	float beatTimer = 0;
	constexpr static float defaultBeatInterval = 1.0f;
	float beatInterval = defaultBeatInterval;

	constexpr static float failDuration = 0.8f;
	float failTime[3] = {0};
	constexpr static float failBlinkDuration = 0.2f;
	bool fail[3] = {0};

	constexpr static int16_t barsY = -5;
	constexpr static uint16_t barsX[3] = {8, 26, 44};
	constexpr static const char* barsIcons[4] = {"/BarP.raw", "/BarY.raw", "/BarB.raw", "/BarR.raw"};

	bool circlesPressed[3] = {false, false, false};
	constexpr static uint16_t circlesY = 104;
	constexpr static uint16_t circlesX[3] = {6, 24, 42};
	constexpr static const char* circlesIcons[3] = {"/circ_p1.raw", "/circ_y1.raw", "/circ_b1.raw"};
	constexpr static const char* circlesIconsPressed[3] = {"/circ_p2.raw", "/circ_y2.raw", "/circ_b2.raw"};


	std::deque<ObjPtr> notes[3];
	constexpr static const char* notesIcons[3] = {"/note1.raw", "/note2.raw", "/note3.raw"};
	constexpr static const char* danceGIFs[4] = {"/dance1.gif", "/dance2.gif", "/dance3.gif", "/dance4.gif"};

	constexpr static float notePerfectY = circlesY + 5;
	constexpr static float noteTolerance = 8.0f;
	void updateNotes(float delta);
	void createNote(uint8_t track);
	constexpr static float defaultNotesSpeed = 50.0f;
	float notesSpeed = defaultNotesSpeed;

	void noteHit(uint8_t track);
	void adjustTempo();
	void adjustScoreBar();
	void buttonPressed(uint i) override;
	void buttonReleased(uint i) override;
};


#endif //CIRCUITPET_FIRMWARE_GAME5_H
