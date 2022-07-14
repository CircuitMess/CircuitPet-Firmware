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
	uint8_t level = 0;
	void nextLevel();
	void gameOver();
	void spawnRandomAsteroid();


	//------------ Player ------------
	Player player;
	uint8_t life = 3;

	void playerHit();
	void updateInvincibility(float delta);
	bool invincible = false;
	float invincibilityTime = 0;
	float invincibilityBlink = 0;
	float invincibilityBlinkDuration = 0.2f;

	constexpr static float invincibilityDuration = 2.0f;
	//------------ Player end ------------


	//------------ Bullet ------------
	struct Bullet {
		std::shared_ptr<GameObject> gObj;
		glm::vec2 velocity;

		bool operator==(const Bullet& other){
			return (velocity == other.velocity) && (gObj == other.gObj);
		}
	};

	constexpr static float bulletSpeed = 80.0f;
	constexpr static uint8_t maxBullets = 4;

	std::vector<Bullet> bulletPool;
	void updateBullets(float deltaTime);
	void shootBullet();
	//------------ Bullet end ------------

	//------------ Asteroid ------------
	enum class AsteroidSize : uint8_t {
		Small, Medium, Large
	};

	struct ImageDesc {
		const char* path;
		PixelDim dim;
	};
	static constexpr ImageDesc asteroidIcons[] = {{"/asteroidS.raw", {15, 14}},
												  {"/asteroidM.raw", {20, 21}},
												  {"/asteroidL.raw", {31, 30}}};

	struct Asteroid {
		std::shared_ptr<GameObject> gObj;
		glm::vec2 velocity;
		AsteroidSize size;

		bool operator==(const Asteroid& other){
			return (velocity == other.velocity) && (gObj == other.gObj) && (size == other.size);
		}
	};

	constexpr static std::array<float, 3> asteroidSpeed = { 25.0f, 20.0f, 12.0f };
	constexpr static std::array<float, 3> asteroidRadius = { 7.5f, 10.0f, 15.0f };
	std::vector<Asteroid> asteroidPool;

	void asteroidHit(const Asteroid& asteroid);
	void createAsteroid(AsteroidSize size, glm::vec2 pos);
	void updateAsteroids(float deltaTime);

	struct {
		GameObject top;
		GameObject bot;
		GameObject left;
		GameObject right;
	} wrapWalls;
	constexpr static glm::vec2 wrapWallsSize = { 160.0f + 4 * asteroidRadius[(uint8_t)AsteroidSize::Large] + 2,
												 128.0f + 4 * asteroidRadius[(uint8_t)AsteroidSize::Large] + 2 };
	//------------ Asteroid end ------------


	void buttonPressed(uint i) override;
	void buttonReleased(uint i) override;

	bool leftHold = false;
	bool rightHold = false;
};


#endif //CIRCUITPET_FIRMWARE_GAME6_H
