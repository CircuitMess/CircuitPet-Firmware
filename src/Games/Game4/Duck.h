
#ifndef CIRCUITPET_FIRMWARE_DUCK_H
#define CIRCUITPET_FIRMWARE_DUCK_H


#include <memory>
#include "../../GameEngine/GameObject.h"
#include "../../GameEngine/Rendering/AnimRC.h"

class Duck {
public:
	Duck(std::shared_ptr <GameObject> duckGO);
	void death();
	void crouch();
	void jump();

	std::shared_ptr<GameObject> getGameObject();
	void setFiles(File walk,
				  File down,
				  File preJump,
				  File midJump,
				  File afterJump);
private:
	std::shared_ptr<GameObject> gameObject;
	std::shared_ptr<AnimRC> animRc;

	File walk;
	File down;
	File preJump;
	File midJump;
	File afterJump;
};


#endif //CIRCUITPET_FIRMWARE_DUCK_H
