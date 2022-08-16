
#ifndef CIRCUITPET_FIRMWARE_DUCK4_H
#define CIRCUITPET_FIRMWARE_DUCK4_H


#include <memory>
#include "../../GameEngine/GameObject.h"
#include "../../GameEngine/Rendering/AnimRC.h"
#include "../../GameEngine/Collision/CircleCC.h"
#include <Input/InputListener.h>


namespace Game4 {
	class Game4;

class Duck : private InputListener {
public:
	Duck(std::shared_ptr<GameObject> duckGoRc, std::shared_ptr<GameObject> duckGoCc, Game4* game4);
	~Duck();
	void death();
	void update(float deltaTime);

	std::shared_ptr<GameObject> getGameObjectCc();
	void setFiles(File walk, File down, File jump, File ducking, File ducked, File unDucking, File up);
	void win();

	bool invincible = false;
private:
	void buttonPressed(uint i) override;
	void buttonReleased(uint i) override;
	void walk();

	void jump();
	void duck();
	void updateInvincibility(float delta);

	Game4* game4;
	std::shared_ptr<GameObject> gameObjectRc;
	std::shared_ptr<GameObject> gameObjectCc;
	std::shared_ptr<AnimRC> animRc;

	const int startPosY = 50;
	float multiplier = 1.0f;
	const float maxHeight = -35.0f;
	const float posXForMaxHeight = 55.0f;
	float time = 0.0f;
	float peakTime;
	float velocity;
	float gravity;
	float invincibilityTime = 0;
	float invincibilityBlink = 0;
	float invincibilityBlinkDuration = 0.2f;
	constexpr static float invincibilityDuration = 2.0f;
	bool isJumping = false;
	bool isDucked = false;
	bool isDone = false;

	File walking;
	File down;
	File jumpAnim;
	File ducking;
	File ducked;
	File unDucking;
	File up;
};
}

#endif //CIRCUITPET_FIRMWARE_DUCK4_H
