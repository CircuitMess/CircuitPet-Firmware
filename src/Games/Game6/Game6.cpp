#include <Pins.hpp>
#include "Game6.h"
#include "../../GameEngine/Rendering/AnimRC.h"
#include "../../GameEngine/Rendering/StaticRC.h"
#include "../../GameEngine/Collision/RectCC.h"
#include "../../GameEngine/Collision/CircleCC.h"
#include "../../GameEngine/Rendering/SpriteRC.h"
#include <Input/Input.h>

Game6::Game6() : Game("", {
		{ "/Bg/Level6.raw",       {}, true },
		{ "/MenuIcons/Icon1.raw", {}, true }}){

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
}

void Game6::onRender(Sprite* canvas){

}

void Game6::onStart(){
	Input::getInstance()->addListener(this);
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

	//TODO - add collision with all asteroids

	collision.wallsAll(*bullet, [this, b](){
		bulletPool.erase(std::remove(bulletPool.begin(), bulletPool.end(), b), bulletPool.end());
		removeObject(b.gObj);
	});
}
