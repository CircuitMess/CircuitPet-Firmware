#include <Pins.hpp>
#include "Game4.h"
#include "../../GameEngine/Rendering/StaticRC.h"
#include "../../GameEngine/Collision/RectCC.h"
#include "../../GameEngine/Collision/PolygonCC.h"
#include <Input/Input.h>

Game4::Game4::Game4() : Game("/Games/Game4", {
		{ "/Background.raw",     {}, true },
		{ "/Goal.raw",           {}, false },
		{ "/TileTop1.raw",       {}, true },
		{ "/TileTop2.raw",       {}, true },
		{ "/TileBot1.raw",       {}, true },
		{ "/TileBot2.raw",       {}, true },
		{ "/ObstacleOver1.raw",  {}, true },
		{ "/ObstacleOver2.raw",  {}, true },
		{ "/ObstacleOver3.raw",  {}, true },
		{ "/ObstacleOver4.raw",  {}, true },
		{ "/ObstacleUnder1.raw", {}, true },
		{ "/ObstacleUnder2.raw", {}, true },
		{ "/ObstacleUnder3.raw", {}, true },
		{ "/DuckDucked.gif",     {}, false },
		{ "/DuckDucking.gif",    {}, false },
		{ "/DuckUnDucking.gif",  {}, false },
		{ "/DuckJump.gif",       {}, false },
		{ "/DuckDown.gif",       {}, false },
		{ "/DuckWin.gif",        {}, false },
		{ "/DuckWalk.gif",       {}, true },
		RES_HEART
}){}


void Game4::Game4::onLoad(){
	setupObstacles();
	leftWallObject = std::make_shared<GameObject>(
			nullptr,
			std::make_unique<RectCC>(glm::vec2{ 10, 128 })
	);
	leftWallObject->setPos({ -55, 0 });
	addObject(leftWallObject);

	///Tiles
	tileManager = std::make_unique<TileManager>(movingTiles);
	tileManager->addFilePair(getFile("/TileTop1.raw"), getFile("/TileBot1.raw"));
	tileManager->addFilePair(getFile("/TileTop2.raw"), getFile("/TileBot2.raw"));
	tileManager->create(); // creates GameObjects into movingTiles

	leftWall = std::make_shared<GameObject>(
			nullptr,
			std::make_unique<RectCC>(glm::vec2{ 1, 128 })
	);
	leftWall->setPos({ -tileDim * tilesPerArray, 0 });
	addObject(leftWall);

	for(int i = 0; i < movingTiles.size(); i++){
		auto obj = movingTiles[i];
		addObject(obj);
		collision.addPair(*leftWall, *obj, [this, i](){ tileManager->reset(i); });
	}
	///Background
	bg = std::make_shared<GameObject>(
			std::make_unique<StaticRC>(getFile("/Background.raw"), PixelDim{ 160, topY }),
			nullptr
	);
	bg->getRenderComponent()->setLayer(-1);
	addObject(bg);
	///Duck
	std::initializer_list<glm::vec2> points = {{ 11, 23 },
											   { 21, 8 },
											   { 31, 16 },
											   { 28, 35 },
											   { 19, 40 },
											   { 9,  34 }};
	auto duckGoCc = std::make_shared<GameObject>(
			nullptr,
			std::make_unique<PolygonCC>(points, glm::vec2{ 23, 27 })
	);
	addObject(duckGoCc);
	duckGoCc->setPos({ 5, 50 });

	auto duckGoRc = std::make_shared<GameObject>(
			std::make_unique<AnimRC>(getFile("/DuckWalk.gif")),
			nullptr
	);
	addObject(duckGoRc);
	duckGoRc->getRenderComponent()->setLayer(10);
	duckGoRc->setPos({ 5, 50 });

	duck = std::make_unique<Duck>(duckGoRc, duckGoCc, this);
	duck->setFiles(getFile("/DuckWalk.gif"),
				   getFile("/DuckDown.gif"),
				   getFile("/DuckJump.gif"),
				   getFile("/DuckDucking.gif"),
				   getFile("/DuckDucked.gif"),
				   getFile("/DuckUnDucking.gif"),
				   getFile("/DuckWin.gif"));
	///Score
	auto scoreObj = std::make_shared<GameObject>(std::make_unique<SpriteRC>(PixelDim{ 70, 7}), nullptr);
	scoreObj->setPos({ 160 - 75 - 1, 2});
	scoreSprite = std::static_pointer_cast<SpriteRC>(scoreObj->getRenderComponent())->getSprite();
	scoreSprite->clear(TFT_TRANSPARENT);
	scoreSprite->setTextColor(TFT_BLACK);
	scoreSprite->setCursor(0, 0);
	scoreSprite->printf("Score:%d/%d", score,scoreMax);
	addObject(scoreObj);
	///Hearts
	hearts = std::make_unique<Hearts>(getFile(FILE_HEART));
	hearts->getGO()->setPos({ 2, 2 });
	addObject(hearts->getGO());

}

void Game4::Game4::onLoop(float deltaTime){
	duck->update(deltaTime);
	for(auto obj: movingObjects){
		float x = obj->getPos().x - deltaTime * speed;
		float y = obj->getPos().y;
		obj->setPos({ x, y });
	}
	for(auto obj: movingTiles){
		float x = obj->getPos().x - deltaTime * speed;
		float y = obj->getPos().y;
		obj->setPos({ x, y });
	}
	if(isDone){
		if(speed == 0) return;
		if(goal->getPos().x <= 58){
			auto diff = 58 - goal->getPos().x;

			for(auto obj: movingObjects){
				float x = obj->getPos().x + diff;
				float y = obj->getPos().y;
				obj->setPos({ x, y });
			}
			for(auto obj: movingTiles){
				float x = obj->getPos().x + diff;
				float y = obj->getPos().y;
				obj->setPos({ x, y });
			}
			duck->win();
			speed = 0;
		}
	}else{
		spawnValue += deltaTime;
		if(spawnValue >= spawnRate){
			spawnValue -= spawnRate;
			spawn();
		}
	}
}

void Game4::Game4::onStart(){
	Input::getInstance()->addListener(this);
}

void Game4::Game4::onStop(){
	Input::getInstance()->removeListener(this);
}

void Game4::Game4::onRender(Sprite* canvas){
	Game::onRender(canvas);
}

void Game4::Game4::buttonPressed(uint i){
	if(isDone) return;

	if(i == BTN_BACK){
		pop();
	}
}


void Game4::Game4::setupObstacles(){
	obstacleOver.push_back({ getFile("/ObstacleOver1.raw"), { 40, 31 }, {{ 0, 30 }, { 25, 0 }, { 39, 0 }}});
	obstacleOver.push_back({ getFile("/ObstacleOver2.raw"), { 24, 19 }, {{ 3, 18 }, { 20, 0 }, { 21, 18 }}});
	obstacleOver.push_back({ getFile("/ObstacleOver3.raw"), { 22, 19 }, {}});
	obstacleOver.push_back({ getFile("/ObstacleOver4.raw"), { 33, 19 }, {{2, 18}, {2, 6}, {10, 1}, {21, 1}, {30, 18}}});

	obstacleUnder.push_back({ getFile("/ObstacleUnder1.raw"), { 36, 27 }, {{ 1, 17 }, { 11, 8 }, { 22, 8 }, { 35, 17 }, { 22, 26 }, { 11, 26 }}});
	obstacleUnder.push_back({ getFile("/ObstacleUnder2.raw"), { 40, 30 }, {{ 0, 20 }, { 9, 11 }, { 27, 20 }, { 9, 29 }}});
	obstacleUnder.push_back({ getFile("/ObstacleUnder3.raw"), { 19, 19 }, {}});
}

void Game4::Game4::spawn(){
	if(speed < speedMax){
		speed += speedIncrement;
		spawnRate -= spawnIncrement;
	}

	if(score >= scoreMax - 1){
		isDone = true;
		goal = std::make_shared<GameObject>(
				std::make_unique<StaticRC>(getFile("/Goal.raw"), PixelDim{ 44, 20 }),
				nullptr
		);
		addObject(goal);
		goal->getRenderComponent()->setLayer(0);
		goal->setPos({ 160, topY - 20 });
		movingObjects.push_back(goal) ;
		return;
	}


	int coinFlip = rand() % 2;
	Obstacle obstacle;
	int posY;
	if(coinFlip == 0){
		int under = rand() % obstacleUnder.size();
		obstacle = obstacleUnder[under];
		posY = topY - 30 - obstacle.dim.y;
	}else{
		int over = rand() % obstacleOver.size();
		obstacle = obstacleOver[over];
		posY = topY - obstacle.dim.y;
	}

	std::shared_ptr<GameObject> gObj;
	if(obstacle.points.size() >= 3){
		gObj = std::make_shared<GameObject>(
				std::make_unique<StaticRC>(obstacle.file, obstacle.dim),
				std::make_unique<PolygonCC>(obstacle.points)
		);
	}else{
		gObj = std::make_shared<GameObject>(
				std::make_unique<StaticRC>(obstacle.file, obstacle.dim),
				std::make_unique<RectCC>(obstacle.dim)
		);
	}
	addObject(gObj);
	gObj->setPos({ 160, posY });
	gObj->getRenderComponent()->setLayer(1);

	collision.addPair(*gObj, *duck->getGameObjectCc(), [this](){
		if(duck->invincible) return;
		duckHit();
	});
	movingObjects.push_back(gObj);
	collision.addPair(*leftWallObject, *gObj, [this](){
		removeObject(movingObjects[0]);
		movingObjects.erase(movingObjects.begin());
		scoreUp();
	});
}

void Game4::Game4::duckHit(){
	life--;
	hearts->setLives(life);
	if(life == 0){
		speed = 0.0f;
		spawnRate = 10000.0f;
		duck->death();
		return;
	}
	duck->invincible = true;
}

float Game4::Game4::getSpeed(){
	return speed;
}

void Game4::Game4::scoreUp(){
	score++;
	scoreSprite->clear(TFT_TRANSPARENT);
	scoreSprite->setTextColor(TFT_BLACK);
	scoreSprite->setCursor(0, 0);
	scoreSprite->printf("Score:%d/%d", score,scoreMax);
}

Stats Game4::Game4::returnStats(){
	return Stats({(uint8_t )(score/2), (uint8_t )score, (uint8_t )(score > 25 ? 25 : score)});
}
