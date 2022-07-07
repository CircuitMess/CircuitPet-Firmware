
#ifndef CIRCUITPET_FIRMWARE_DUCK_H
#define CIRCUITPET_FIRMWARE_DUCK_H


#include <Input/InputListener.h>
#include "../../GameEngine/GameObject.h"
#include "../../GameEngine/Rendering/AnimRC.h"


class Duck : private InputListener {
public:
	Duck(File walk, File eat);
	~Duck();
	std::shared_ptr<GameObject> getGameObject();
	void loop(float deltaTime);
private:
	void buttonPressed(uint i) override;
	void buttonReleased(uint i) override;

	const uint16_t width = 48;
	const uint16_t height = 77;
	float velocity = 0.0f;
	const float speed = 30.0f;
	const float beakRadius = (float)width/4;

	File walk;
	File eat;

	std::shared_ptr<GameObject> go;
	std::shared_ptr<AnimRC> anim;
};


#endif //CIRCUITPET_FIRMWARE_DUCK_H
