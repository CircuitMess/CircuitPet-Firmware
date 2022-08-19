#include "Game2.h"
#include "../../GameEngine/Rendering/StaticRC.h"
#include "../../GameEngine/Rendering/SpriteRC.h"
#include "../../GameEngine/Rendering/AnimRC.h"
#include "../../GameEngine/Collision/CircleCC.h"
#include "../../GameEngine/Collision/RectCC.h"
#include "../../GameEngine/Collision/PolygonCC.h"
#include <Pins.hpp>
#include <Input/Input.h>
#include <glm/gtx/vector_angle.hpp>
#include "../../RGBController.h"

constexpr Game2::ObstacleDesc Game2::TopObstacles[];
constexpr Game2::ObstacleDesc Game2::BotObstacles[];

Game2::Game2() : Game("/Games/2", {
		{ "/duck.gif", {}, true },
		{ "/bg.raw", {}, true },
		RES_HEART,
		RES_GOBLET,
		{ TopObstacles[0].path, {}, true },
		{ TopObstacles[1].path, {}, true },
		{ BotObstacles[0].path, {}, true },
		{ BotObstacles[1].path, {}, true },
		{ BotObstacles[2].path, {}, true },
		{ BotObstacles[3].path, {}, true },
		{ BotObstacles[4].path, {}, true }}){

}

void Game2::onLoad(){
	auto bg = std::make_shared<GameObject>(
			std::make_unique<StaticRC>(getFile("/bg.raw"), PixelDim{ 160, 128 }),
			nullptr
	);
	bg->getRenderComponent()->setLayer(-1);
	addObject(bg);

	hearts = std::make_unique<Hearts>(getFile(FILE_HEART));
	hearts->getGO()->setPos({ 2, 2 });
	addObject(hearts->getGO());

	scoreDisplay = std::make_unique<Score>(getFile(FILE_GOBLET));
	scoreDisplay->getGO()->setPos({ 160 - 2 - 28, 2 });
	addObject(scoreDisplay->getGO());


	duck = std::make_shared<GameObject>(
			std::make_unique<AnimRC>(getFile("/duck.gif")),
			std::make_unique<CircleCC>(8, glm::vec2{ 7, 7 })
	);

	anim = std::static_pointer_cast<AnimRC>(duck->getRenderComponent());
	anim->setLoopMode(GIF::Single);

	addObject(duck);
	resetDuck();
}

void Game2::onStart(){
	Input::getInstance()->addListener(this);

	anim->start();
}

void Game2::onStop(){
	Input::getInstance()->removeListener(this);

	anim->stop();
	anim->reset();
}

void Game2::onRender(Sprite* canvas){
}

void Game2::onLoop(float deltaTime){
	updateDuck(deltaTime);
	updateObstacles(deltaTime);

	if(state == Play){
		if(obstacles.empty() || obstacles.back().top->getPos().x <= 128 - obstacleSpacing){
			createObstaclePair();
		}
	}

	if(!obstacles.empty()){
		if(obstacles.front().bot->getPos().x < -40){
			removeObject(obstacles.front().top);
			removeObject(obstacles.front().bot);
			obstacles.pop_front();
		}
	}

	for(auto& obstacle: obstacles){
		if(obstacle.top->getPos().x + 15 <= duckPosX && !obstacle.passed && state == Play){
			RGBSlot.blink(Pixel::Green);
			score++;
			obstacle.passed = true;
			scoreDisplay->setScore(score);
		}
	}

	if(state == FallOut && obstacles.empty()){
		if(life == 0){
			pop();
		}else{
			resetDuck();
		}
	}
}

void Game2::updateDuck(float delta){
	if(state != FlyIn && state != Play && state != FallOut) return;

	glm::vec2 pos = duck->getPos();

	if(state == FlyIn){
		entry += delta / entryDuration;
		float tX = entry;
		float tY = 1.f - pow(max(0.0f, 1.f - (entry * 1.1f)), 2);

		glm::vec2 newPos = { duckPosX * tX, -20 + (64 + 20) * tY };
		duck->setPos(newPos);

		pos.x -= entry * duckPosX;
		glm::vec2 dir = newPos - pos;
		float angle = glm::orientedAngle(glm::normalize(dir), glm::vec2{ 1, 0 });
		duck->setRot(-glm::degrees(angle));

		newPos.x = duckPosX;
		duck->setPos(newPos);

		if(entry >= 1){
			duck->setPos({ duckPosX, 64 });
			duck->setRot(0);
			velocity.y = 0;
			state = Wait;
		}

		return;
	}

	velocity += delta * glm::vec2{ 0, gravity };
	pos += delta * velocity;
	pos.x = duckPosX;

	if(pos.y < 0){
		velocity.y = 0;
		pos.y = 0;
	}

	float angle = glm::orientedAngle(glm::normalize(velocity), glm::vec2{ 1, 0 });
	angle = -glm::degrees(angle);

	duck->setRot(angle);
	duck->setPos(pos);
}

void Game2::updateObstacles(float delta){
	auto move = [delta](ObjPtr& obj){
		glm::vec2 pos = obj->getPos();
		pos.x -= delta * speedX;
		obj->setPos(pos);
	};

	for(auto& obstacle: obstacles){
		move(obstacle.top);
		move(obstacle.bot);
	}
}

void Game2::buttonPressed(uint i){
	if(i == BTN_BACK){
		pop();
		return;
	}else if(i != BTN_LEFT) return;

	if(state == Wait || state == FlyIn){
		anim->setLoopMode(GIF::Single);
		state = Play;
	}

	if(state != Play) return;

	anim->reset();
	anim->start();

	velocity.y = -flapSpeedY;
}

void Game2::resetDuck(){
	duck->setPos({ 0, -20 });
	duck->setRot(0);
	collision.wallBot(*duck, [this](){ die(); });

	anim->setLoopMode(GIF::Infinite);
	velocity.y = 0;
	state = FlyIn;
	entry = 0;
}

void Game2::createObstaclePair(){
	int topi = rand() % (sizeof(TopObstacles) / sizeof(TopObstacles[0]));
	int boti = rand() % (sizeof(BotObstacles) / sizeof(BotObstacles[0]));

	auto topDesc = TopObstacles[topi];
	auto botDesc = BotObstacles[boti];

	auto topObj = std::make_shared<GameObject>(
			std::make_unique<StaticRC>(getFile(topDesc.path), topDesc.dim),
			std::make_unique<PolygonCC>(topDesc.collision)
	);

	auto botObj = std::make_shared<GameObject>(
			std::make_unique<StaticRC>(getFile(botDesc.path), botDesc.dim),
			std::make_unique<PolygonCC>(botDesc.collision)
	);

	int offsetBoth = rand() % 40;
	int offsetTop = rand() % 10;
	int offsetBot = rand() % 20;

	topObj->setPos({ 160, offsetBoth + offsetTop });
	botObj->setPos({ 160 + 15, (128 - botDesc.dim.y) + 10 + offsetBoth + offsetBot });

	collision.addPair(*duck, *topObj, [this](){ die(); });
	collision.addPair(*duck, *botObj, [this](){ die(); });

	addObject(topObj);
	addObject(botObj);
	obstacles.push_back({ topObj, botObj, false });
}

void Game2::die(){
	if(state != Play) return;
	RGBSlot.blink(Pixel::Red);

	life--;
	hearts->setLives(life);

	state = FallOut;

	collision.wallBot(*duck, {});

	for(const auto& obstacle: obstacles){
		collision.removePair(*duck, *obstacle.top);
		collision.removePair(*duck, *obstacle.bot);
	}
}

Stats Game2::returnStats(){
	return Stats({ (uint8_t )(score > 25 ? 50 : score * 2),(uint8_t ) (score > 50 ? 100 : score * 2), (uint8_t )(score > 15 ? 15 : score) });
}
