#include <Pins.hpp>
#include "Game6.h"
#include "../../GameEngine/Rendering/StaticRC.h"
#include "../../GameEngine/Collision/RectCC.h"
#include "../../GameEngine/Collision/CircleCC.h"
#include "../../GameEngine/Rendering/SpriteRC.h"
#include <Input/Input.h>

//compiler says declarations are required, not redundant
constexpr std::array<float, 3> Game6::asteroidSpeed;
constexpr std::array<float, 3> Game6::asteroidRadius;

Game6::Game6() : wrapWalls({ .top =  { nullptr, std::make_unique<RectCC>(glm::vec2 { wrapWallsSize.x, 100 }) },
							 .bot =  { nullptr, std::make_unique<RectCC>(glm::vec2 { wrapWallsSize.x, 100 }) },
							 .left =  { nullptr, std::make_unique<RectCC>(glm::vec2 { 100, wrapWallsSize.y }) },
							 .right =  { nullptr, std::make_unique<RectCC>(glm::vec2 { 100, wrapWallsSize.y }) }
						   }),
				 Game("", {{ "/Bg/Level6.raw",       {}, true },
						 { "/MenuIcons/Icon1.raw", {}, true },
						   { "/MenuIcons/Icon2.raw", {}, true }
				 }){

	wrapWalls.top.setPos(glm::vec2{ 0, -100 } - (2*asteroidRadius[(uint8_t)AsteroidSize::Large] + 1));
	wrapWalls.bot.setPos(glm::vec2{ 0, 128 } + (2*asteroidRadius[(uint8_t)AsteroidSize::Large] + 1));
	wrapWalls.left.setPos(glm::vec2{ -100, 0 } - (2*asteroidRadius[(uint8_t)AsteroidSize::Large] + 1));
	wrapWalls.right.setPos(glm::vec2{ 160, 0 } + (2*asteroidRadius[(uint8_t)AsteroidSize::Large] + 1));
}

void Game6::onLoad(){
	auto pat = std::make_shared<GameObject>(std::make_unique<StaticRC>(getFile("/MenuIcons/Icon1.raw"), PixelDim { 32, 32 }),
											std::make_unique<CircleCC>(16));
	addObject(pat);
	player.setObj(pat);
	pat->setPos({ 80, 60 });


	auto bg = std::make_shared<GameObject>(
			std::make_unique<StaticRC>(getFile("/Bg/Level6.raw"), PixelDim { 160, 128 }),
			nullptr
	);
	addObject(bg);
	bg->getRenderComponent()->setLayer(-1);
}

void Game6::onLoop(float deltaTime){

	if(leftHold && !rightHold){
		player.leftTurn(deltaTime);
	}else if(rightHold && !leftHold){
		player.rightTurn(deltaTime);
	}

	updateBullets(deltaTime);
	updateAsteroids(deltaTime);
}

void Game6::onRender(Sprite* canvas){

}

void Game6::onStart(){
	Input::getInstance()->addListener(this);

	createAsteroid(AsteroidSize::Large, {-(2*asteroidRadius[(uint8_t)AsteroidSize::Large]), 50});
	createAsteroid(AsteroidSize::Medium, {160, 50});
	createAsteroid(AsteroidSize::Small, {-(2*asteroidRadius[(uint8_t)AsteroidSize::Large]), 100});
}

void Game6::onStop(){
	Input::getInstance()->removeListener(this);
}

void Game6::buttonPressed(uint i){
	switch(i){
		case BTN_BACK:
			pop();
			break;

		case BTN_LEFT:
			leftHold = true;
			break;

		case BTN_RIGHT:
			rightHold = true;
			break;

		case BTN_ENTER:
			shootBullet();
			break;
	}
}

void Game6::buttonReleased(uint i){
	switch(i){
		case BTN_LEFT:
			leftHold = false;
			break;

		case BTN_RIGHT:
			rightHold = false;
			break;
	}
}

void Game6::updateBullets(float deltaTime){
	for(auto& bullet : bulletPool){
		bullet.gObj->setPos(bullet.gObj->getPos() + bullet.velocity * deltaTime);
	}
}

void Game6::shootBullet(){
	if(bulletPool.size() >= 4) return;

	auto spriteRC = std::make_unique<SpriteRC>(PixelDim { 4, 4 });
	spriteRC->getSprite()->clear(TFT_TRANSPARENT);
	spriteRC->getSprite()->fillRoundRect(0, 0, 4, 4, 1, TFT_WHITE);

	auto bullet = std::make_shared<GameObject>(std::move(spriteRC),
											   std::make_unique<CircleCC>(2, glm::vec2 { 2, 2 }));
	addObject(bullet);

	glm::vec2 center = player.getObj()->getPos() + 16.0f;
	glm::vec2 direction = { cos(M_PI * (player.getAngle() - 90.f) / 180.0), sin(M_PI * (player.getAngle() - 90.f) / 180.0) };
	glm::vec2 bulletPos = direction * 16.0f + center;
	glm::vec2 speed = direction * bulletSpeed;

	bullet->setPos(bulletPos);

	Bullet b = { bullet, speed };
	bulletPool.push_back(b);

	for(auto &asteroid: asteroidPool){
		collision.addPair(*asteroid.gObj, *bullet, [this, b, asteroid](){
			bulletPool.erase(std::remove(bulletPool.begin(), bulletPool.end(), b), bulletPool.end());
			removeObject(b.gObj);

			asteroidPool.erase(std::remove(asteroidPool.begin(), asteroidPool.end(), asteroid), asteroidPool.end());
			removeObject(asteroid.gObj);
		});
	}

	collision.wallsAll(*bullet, [this, b](){
		bulletPool.erase(std::remove(bulletPool.begin(), bulletPool.end(), b), bulletPool.end());
		removeObject(b.gObj);
	});
}

void Game6::createAsteroid(Game6::AsteroidSize size, glm::vec2 pos){

	auto spriteRC = std::make_unique<SpriteRC>(PixelDim { asteroidRadius[(uint8_t)size] * 2, asteroidRadius[(uint8_t)size] * 2 });
	spriteRC->getSprite()->clear(TFT_TRANSPARENT);
	spriteRC->getSprite()->fillCircle(asteroidRadius[(uint8_t)size], asteroidRadius[(uint8_t)size], asteroidRadius[(uint8_t)size], TFT_BROWN);

	auto asteroid = std::make_shared<GameObject>(std::move(spriteRC),
												 std::make_unique<CircleCC>(asteroidRadius[(uint8_t)size],
																			glm::vec2 { asteroidRadius[(uint8_t)size], asteroidRadius[(uint8_t)size] }));
	addObject(asteroid);
	asteroid->setPos(pos);

	float angle = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 360.0f));
	glm::vec2 direction = { cos(M_PI * angle / 180.0), sin(M_PI * angle / 180.0) };
	glm::vec2 speed = direction * asteroidSpeed[(uint8_t)size];

	Asteroid a = { asteroid, speed, size };
	asteroidPool.push_back(a);

	for(auto &bullet: bulletPool){
		collision.addPair(*asteroid, *bullet.gObj, [this, bullet, a](){
			bulletPool.erase(std::remove(bulletPool.begin(), bulletPool.end(), bullet), bulletPool.end());
			removeObject(bullet.gObj);

			asteroidPool.erase(std::remove(asteroidPool.begin(), asteroidPool.end(), a), asteroidPool.end());
			removeObject(a.gObj);
		});
	}



	collision.addPair(*asteroid, wrapWalls.top, [asteroid](){
		asteroid->setPos({ asteroid->getPos().x, 128.0f });
	});
	collision.addPair(*asteroid, wrapWalls.bot, [asteroid](){
		asteroid->setPos({ asteroid->getPos().x, -(2 * asteroidRadius[(uint8_t)AsteroidSize::Large]) });
	});
	collision.addPair(*asteroid, wrapWalls.left, [asteroid](){
		asteroid->setPos({ 160.0f, asteroid->getPos().y });
	});
	collision.addPair(*asteroid, wrapWalls.right, [asteroid](){
		asteroid->setPos({ -(2 * asteroidRadius[(uint8_t)AsteroidSize::Large]), asteroid->getPos().y });
	});
}

void Game6::updateAsteroids(float deltaTime){
	for(auto& asteroid : asteroidPool){
		asteroid.gObj->setPos(asteroid.gObj->getPos() + asteroid.velocity * deltaTime);
	}
}
