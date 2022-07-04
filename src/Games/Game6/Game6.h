#ifndef CIRCUITPET_FIRMWARE_GAME6_H
#define CIRCUITPET_FIRMWARE_GAME6_H

#include <Arduino.h>
#include <Input/InputListener.h>
#include "../../GameEngine/Game.h"
#include "Player.h"

class Game6 : public Game, private InputListener {
protected:
public:
	Game6();

protected:
	void onLoad() override;
	void onLoop(float deltaTime) override;
	void onStart() override;
	void onStop() override;
	void onRender(Sprite* canvas) override;

private:
	Player player;

	struct Bullet {
		std::shared_ptr<GameObject> gObj;
		glm::vec2 velocity;

		bool operator==(const Bullet& other){
			return (velocity == other.velocity) && (gObj == other.gObj);
		}
	};
	constexpr static float bulletSpeed = 50.0f;
	constexpr static uint8_t maxBullets = 4;

	std::vector<Bullet> bulletPool;
	void updateBullets(float deltaTime);
	void shootBullet();

	void buttonPressed(uint i) override;
	void buttonReleased(uint i) override;

	bool leftHold = false;
	bool rightHold = false;

};


#endif //CIRCUITPET_FIRMWARE_GAME6_H
