#include "Game2.h"
#include "../../GameEngine/Rendering/StaticRC.h"
#include "../../GameEngine/Rendering/AnimRC.h"
#include "../../GameEngine/Collision/CircleCC.h"
#include "../../GameEngine/Collision/RectCC.h"
#include <Pins.hpp>
#include <Input/Input.h>

constexpr Game2::ImageDesc Game2::upObstacles[];
constexpr Game2::ImageDesc Game2::downObstacles[];

Game2::Game2() : Game("/Games/2", {
		{ "/duck.gif", {}, true },
		{ "/bg.raw",   {}, true },
		{ upObstacles[0].path,   {}, true },
		{ upObstacles[1].path,   {}, true },
		{ downObstacles[0].path,   {}, true },
		{ downObstacles[1].path,   {}, true },
		{ downObstacles[2].path,   {}, true },
		{ downObstacles[3].path,   {}, true },
		{ downObstacles[4].path,   {}, true }}),
		leftWall(nullptr, std::make_unique<RectCC>(glm::vec2 { 20, 128 })){

	leftWall.setPos({-(20 + 50), 0});
}

void Game2::onLoad(){
	auto bg = std::make_shared<GameObject>(
			std::make_unique<StaticRC>(getFile("/bg.raw"), PixelDim { 160, 128 }), nullptr);
	bg->getRenderComponent()->setLayer(-1);
	addObject(bg);

	duck = std::make_shared<GameObject>(std::make_unique<AnimRC>(getFile("/duck.gif")),
										std::make_unique<CircleCC>(8, glm::vec2 { 7, 7 }));
	duck->setPos({ startingX, startingY });
	addObject(duck);

	collision.wallBot(*duck, [this](){
		gameOver();
	});
}

void Game2::onLoop(float deltaTime){
	if(!firstPress) return;

	updateDuck(deltaTime);
	updateObstacles(deltaTime);
}

void Game2::onStart(){
	Input::getInstance()->addListener(this);
	std::static_pointer_cast<AnimRC>(duck->getRenderComponent())->start();
}

void Game2::onStop(){
	Input::getInstance()->removeListener(this);
	std::static_pointer_cast<AnimRC>(duck->getRenderComponent())->stop();
}

void Game2::onRender(Sprite* canvas){
}

void Game2::buttonPressed(uint i){
	if(i == BTN_B){
		pop();
	}else if(i == BTN_LEFT){
		if(!firstPress){
			firstPress = true;
		}

		if(duckSpeed < 0){
			duckSpeed = max(duckSpeed - flapSpeed, -3 * flapSpeed);
		}else{
			duckSpeed = -flapSpeed;
		}
	}

}

void Game2::gameOver(){
	pop();
}

void Game2::updateDuck(float delta){
	float y = duck->getPos().y;

	duckSpeed += gravity * delta;

	y += duckSpeed * delta;

	if(y < 0.f){
		duckSpeed = 0;
		y = 0;
	}


	duck->setPos({ duck->getPos().x, y });
}

void Game2::updateObstacles(float delta){


	for(auto obstacle : obstacles){
		obstacle->setPos({ obstacle->getPos().x - obstacleSpeed * delta, obstacle->getPos().y });
	}
}
