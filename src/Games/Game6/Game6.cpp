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
constexpr Game6::ImageDesc Game6::asteroidIcons[];

Game6::Game6() : wrapWalls({ .top =  { nullptr, std::make_unique<RectCC>(glm::vec2 { wrapWallsSize.x, 100 }) },
							 .bot =  { nullptr, std::make_unique<RectCC>(glm::vec2 { wrapWallsSize.x, 100 }) },
							 .left =  { nullptr, std::make_unique<RectCC>(glm::vec2 { 100, wrapWallsSize.y }) },
							 .right =  { nullptr, std::make_unique<RectCC>(glm::vec2 { 100, wrapWallsSize.y }) }
						   }),
				 Game("/Games/6", {{ "/bg.raw",       {}, true },
						 { asteroidIcons[0].path, {}, true },
						 { asteroidIcons[1].path, {}, true },
						 { asteroidIcons[2].path, {}, true },
				 }){

	wrapWalls.top.setPos(glm::vec2 { 0, -100 } - (2 * asteroidRadius[(uint8_t)AsteroidSize::Large] + 1));
	wrapWalls.bot.setPos(glm::vec2 { -(2 * asteroidRadius[(uint8_t)AsteroidSize::Large] + 1), 128 + (2 * asteroidRadius[(uint8_t)AsteroidSize::Large] + 1) });
	wrapWalls.left.setPos(glm::vec2 { -100, 0 } - (2 * asteroidRadius[(uint8_t)AsteroidSize::Large] + 1));
	wrapWalls.right.setPos(glm::vec2 { 160 + (2 * asteroidRadius[(uint8_t)AsteroidSize::Large] + 1), -(2 * asteroidRadius[(uint8_t)AsteroidSize::Large] + 1) });
}

void Game6::onLoad(){
	auto pat = std::make_shared<GameObject>(std::make_unique<StaticRC>(getFile(asteroidIcons[2].path),  asteroidIcons[2].dim),
											std::make_unique<CircleCC>(16, glm::vec2{16, 16}));
	addObject(pat);
	player.setObj(pat);
	pat->setPos({ 80, 60 });


	auto bg = std::make_shared<GameObject>(
			std::make_unique<StaticRC>(getFile("/bg.raw"), PixelDim { 160, 128 }),
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

	updateInvincibility(deltaTime);
	updateBullets(deltaTime);
	updateAsteroids(deltaTime);


	if(asteroidPool.empty()){
		nextLevel();
	}
}

void Game6::onRender(Sprite* canvas){

}

void Game6::onStart(){
	Input::getInstance()->addListener(this);

	nextLevel();
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

			asteroidHit(asteroid);
		});
	}

	collision.wallsAll(*bullet, [this, b](){
		bulletPool.erase(std::remove(bulletPool.begin(), bulletPool.end(), b), bulletPool.end());
		removeObject(b.gObj);
	});
}

void Game6::createAsteroid(Game6::AsteroidSize size, glm::vec2 pos){

	auto spriteRC = std::make_unique<StaticRC>(getFile(asteroidIcons[(uint8_t)size].path), asteroidIcons[(uint8_t)size].dim);
	auto asteroid = std::make_shared<GameObject>(std::move(spriteRC),
												 std::make_unique<CircleCC>(asteroidRadius[(uint8_t)size],
																			glm::vec2 { asteroidRadius[(uint8_t)size], asteroidRadius[(uint8_t)size] }));
	addObject(asteroid);
	asteroid->setPos(pos);

	//random direction, avoid right angles since they can keep the asteroids off-screen for long durations
	float angle = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 360.0f));
	float rightAngleOffset = 15;
	if(fmod(angle, 90) <= rightAngleOffset){
		angle += rightAngleOffset;
	}else if(fmod(angle, 90) >= 90 - rightAngleOffset){
		angle -= rightAngleOffset;
	}

	glm::vec2 direction = { cos(M_PI * angle / 180.0), sin(M_PI * angle / 180.0) };
	glm::vec2 speed = direction * asteroidSpeed[(uint8_t)size];

	Asteroid a = { asteroid, speed, size };
	asteroidPool.push_back(a);

	for(auto &bullet: bulletPool){
		collision.addPair(*asteroid, *bullet.gObj, [this, bullet, a](){
			bulletPool.erase(std::remove(bulletPool.begin(), bulletPool.end(), bullet), bulletPool.end());
			removeObject(bullet.gObj);

			asteroidHit(a);
		});
	}

	collision.addPair(*asteroid, *player.getObj(), [this, a](){
		if(invincible) return;

		asteroidHit(a);
		playerHit();
	});


	//wrapping around screen
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

void Game6::asteroidHit(const Game6::Asteroid& asteroid){
	switch(asteroid.size){
		case AsteroidSize::Large:
			createAsteroid(AsteroidSize::Medium, asteroid.gObj->getPos() + (asteroidRadius[(uint8_t)AsteroidSize::Large] - asteroidRadius[(uint8_t)AsteroidSize::Medium]));
			createAsteroid(AsteroidSize::Medium, asteroid.gObj->getPos() + (asteroidRadius[(uint8_t)AsteroidSize::Large] - asteroidRadius[(uint8_t)AsteroidSize::Medium]));
			break;
		case AsteroidSize::Medium:
			createAsteroid(AsteroidSize::Small, asteroid.gObj->getPos() + (asteroidRadius[(uint8_t)AsteroidSize::Medium] - asteroidRadius[(uint8_t)AsteroidSize::Small]));
			createAsteroid(AsteroidSize::Small, asteroid.gObj->getPos() + (asteroidRadius[(uint8_t)AsteroidSize::Medium] - asteroidRadius[(uint8_t)AsteroidSize::Small]));
			break;
		case AsteroidSize::Small:
			break;

	}
	asteroidPool.erase(std::remove(asteroidPool.begin(), asteroidPool.end(), asteroid), asteroidPool.end());
	removeObject(asteroid.gObj);
}

void Game6::updateInvincibility(float delta){
	if(!invincible) return;

	invincibilityTime += delta;

	if((int)(invincibilityTime / invincibilityBlinkDuration) % 2 == 0){
		player.getObj()->getRenderComponent()->setVisible(false);
	}else{
		player.getObj()->getRenderComponent()->setVisible(true);
	}

	if(invincibilityTime >= invincibilityDuration){
		invincibilityTime = 0;
		invincible = false;
		player.getObj()->getRenderComponent()->setVisible(true);
	}
}

void Game6::playerHit(){
	life--;
	if(life == 0){
		gameOver();
	}
	invincible = true;
}

void Game6::nextLevel(){
	level++;

	for(uint8_t i = 0; i < level; i++){
		spawnRandomAsteroid();
	}
}

void Game6::spawnRandomAsteroid(){
	glm::vec2 pos;
	//New asteroids will only be spawned barely outside the screen area:
	//x in range (-2*asteroidRadius[Large], 160), y either -2*asteroidRadius[Large] or 128
	//y in range (-2*asteroidRadius[Large], 128), x either -2*asteroidRadius[Large] or 160

	//top left corner of rectangle outside of the screen, wider by 2*radius[Large] than screen
	glm::vec2 topLeft = {-2*asteroidRadius[(uint8_t)AsteroidSize::Large], -2*asteroidRadius[(uint8_t)AsteroidSize::Large]};

	//pick border for asteroid to spawn on
	enum class Border : uint8_t {
		Up, Down, Left, Right
	} side = static_cast<Border>(rand() % 4);

	if(side == Border::Up || side == Border::Down){
		float xpos = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (160.0f  - topLeft.x)));

		if(side == Border::Up){
			pos = {topLeft.x + xpos, topLeft.y};
		}else if(side == Border::Down){
			pos = {topLeft.x + xpos, 128.0f};
		}
	}else if(side == Border::Left ||side == Border::Right){
		float ypos = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (128.0f  - topLeft.y)));

		if(side == Border::Left){
			pos = {topLeft.x, topLeft.y + ypos};
		}else if(side == Border::Right){
			pos = {160.0f, topLeft.y + ypos};
		}
	}

	createAsteroid(AsteroidSize::Large, pos);
}

void Game6::gameOver(){
	//play anim
//	pop();

}