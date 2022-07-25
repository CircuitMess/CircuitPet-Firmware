
#ifndef CIRCUITPET_FIRMWARE_DUCK_H
#define CIRCUITPET_FIRMWARE_DUCK_H


#include <memory>
#include "../../GameEngine/GameObject.h"
#include "../../GameEngine/Rendering/AnimRC.h"
#include "../../GameEngine/Collision/CircleCC.h"
#include <Input/InputListener.h>


class Game4;

class Duck : private InputListener {
public:
	Duck(std::shared_ptr<GameObject> duckGO, Game4* game4);
	~Duck();
	void death();
	void update(float deltaTime);

	std::shared_ptr<GameObject> getGameObject();
	void setFiles(File walk, File down, File preJump, File midJump, File afterJump, File ducking, File ducked, File unDucking);

private:
	void buttonPressed(uint i) override;
	void buttonReleased(uint i) override;
	void walk();

	Game4* game4;
	std::shared_ptr<GameObject> gameObject;
	std::shared_ptr<AnimRC> animRc;
	std::shared_ptr<CircleCC> circleCc;

	const int startPosY = 50;
	float velocity;
	float multiplier = 1.0f;
	const float jumpVelocity = -45.0f;
	const float gravity = -jumpVelocity/1.8f;
	bool isJumping = false;
	File walking;
	File down;
	File preJump;
	File midJump;
	File afterJump;
	File ducking;
	File ducked;
	File unDucking;
};


#endif //CIRCUITPET_FIRMWARE_DUCK_H
