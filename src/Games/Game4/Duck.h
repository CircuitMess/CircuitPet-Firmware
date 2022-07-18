
#ifndef CIRCUITPET_FIRMWARE_DUCK_H
#define CIRCUITPET_FIRMWARE_DUCK_H


#include <memory>
#include "../../GameEngine/GameObject.h"
#include "../../GameEngine/Rendering/AnimRC.h"
#include "../../GameEngine/Collision/CircleCC.h"


class Game4;

class Duck {
public:
	Duck(std::shared_ptr<GameObject> duckGO, Game4* game4);
	void death();
	void crouch();
	void jump();
	void walk();

	std::shared_ptr<GameObject> getGameObject();
	void setFiles(File walk, File down, File preJump, File midJump, File afterJump, File crouching);
private:
	std::shared_ptr<GameObject> gameObject;
	std::shared_ptr<AnimRC> animRc;
	std::shared_ptr<CircleCC> circleCc;

	Game4* game4;

	File walking;
	File down;
	File preJump;
	File midJump;
	File afterJump;
	File crouching;
};


#endif //CIRCUITPET_FIRMWARE_DUCK_H
