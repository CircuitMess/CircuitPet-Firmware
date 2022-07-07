
#ifndef CIRCUITPET_FIRMWARE_DUCK_H
#define CIRCUITPET_FIRMWARE_DUCK_H


#include <Input/InputListener.h>
#include "../../GameEngine/GameObject.h"

class Duck : private InputListener {
public:
	Duck();
	~Duck();
	std::shared_ptr<GameObject> getGameObject();
	void loop(float deltaTime);
private:
	void buttonPressed(uint i) override;
	void buttonReleased(uint i) override;

	const uint16_t width = 40;
	const uint16_t height = 60;
	float velocity = 0.0f;
	const float speed = 20.0f;

	std::shared_ptr<GameObject> go;
};


#endif //CIRCUITPET_FIRMWARE_DUCK_H
