
#ifndef CIRCUITPET_FIRMWARE_DUCK_H
#define CIRCUITPET_FIRMWARE_DUCK_H


#include <Input/InputListener.h>
#include "../../GameEngine/GameObject.h"
#include "../../GameEngine/Rendering/AnimRC.h"


class Duck : private InputListener {
public:
	Duck(File walk, File eat, File eatBad, File win);
	~Duck();
	std::shared_ptr<GameObject> getGameObject();
	void loop(float deltaTime);
	void startEating(int value);
	void filled();

private:
	void buttonPressed(uint i) override;
	void buttonReleased(uint i) override;
	void finishEating();
	const uint16_t width = 48;
	const uint16_t height = 77;
	float velocity = 0.0f;
	const float speed = 50.0f;
	const float beakRadius = (float)width/5;
	bool eating = false;

	File walk;
	File eat;
	File eatBad;
	File win;

	std::shared_ptr<GameObject> go;
	std::shared_ptr<AnimRC> anim;
};


#endif //CIRCUITPET_FIRMWARE_DUCK_H
