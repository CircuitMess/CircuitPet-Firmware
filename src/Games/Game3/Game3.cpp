#include <Pins.hpp>
#include "Game3.h"
#include "../../GameEngine/Rendering/SpriteRC.h"
#include "../../GameEngine/Rendering/StaticRC.h"
#include "../../GameEngine/Collision/RectCC.h"
#include <Input/Input.h>
#include <time.h>

Game3::Game3() : Game("/Games/Game3", {
		{ "/Background.raw", {}, true },
		{ "/Bomb.raw",       {}, true },
		{ "/Dynamite.raw",   {}, true },
		{ "/Nut.raw",        {}, true },
		{ "/Screw.raw",      {}, true },
		{ "/BarFrame.raw",   {}, true },
		{ "/Heart.raw",      {}, true },
		{ "/DuckWalk.gif",   {}, true },
		{ "/DuckEatBad.gif", {}, true },
		{ "/DuckWin.gif",    {}, true },
		{ "/DuckEat.gif",    {}, true }
}){}

void Game3::onLoad(){
	srand(time(NULL));

	bg = std::make_shared<GameObject>(
			std::make_unique<StaticRC>(getFile("/Background.raw"), PixelDim{ 160, 128 }),
			nullptr
	);
	addObject(bg);
	bg->getRenderComponent()->setLayer(-2);

	collectorBot = std::make_shared<GameObject>(
			nullptr,
			std::make_unique<RectCC>(PixelDim{ 160, 15 })
	);
	collectorBot->setPos({ 0, 160 });
	addObject(collectorBot);

	duck = std::make_unique<Duck>(getFile("/DuckWalk.gif"), getFile("/DuckEat.gif"), getFile("/DuckEatBad.gif"), getFile("/DuckWin.gif"));
	addObject(duck->getGameObject());
	duck->getGameObject()->getRenderComponent()->setLayer(1);

	addTemplate("/Nut.raw", PixelDim{ 13, 13 }, 10);
	addTemplate("/Screw.raw", PixelDim{ 5, 15 }, 5);
	addTemplate("/Bomb.raw", PixelDim{ 14, 22 }, 0);
	addTemplate("/Dynamite.raw", PixelDim{ 5, 20 }, 0);

	auto heartsRC = std::make_unique<SpriteRC>(PixelDim{ 24, 6 });
	hearts = heartsRC->getSprite();
	auto heartsGO = std::make_shared<GameObject>(
			move(heartsRC),
			nullptr
	);
	addObject(heartsGO);
	heartsGO->setPos({ 10, 5 });
	heartsGO->getRenderComponent()->setLayer(0);
	drawHearts();

	auto barRC = std::make_unique<SpriteRC>(PixelDim{ 8, 122 });
	hungerBar = barRC->getSprite();
	auto barGO = std::make_shared<GameObject>(
			move(barRC),
			nullptr
	);
	addObject(barGO);
	barGO->setPos({ 1, 2 });
	hungerBar->drawIcon(getFile("/BarFrame.raw"), 0, 0, 8, 122);
}

void Game3::onLoop(float deltaTime){
	if(dead){
		return;
	}

	duck->loop(deltaTime);
	timeToSpawn += deltaTime;
	if(timeToSpawn >= spawnRate){
		spawnRandom();
		timeToSpawn -= spawnRate;
	}

	if(movingObjects.empty()) return;

	for(const auto &object: movingObjects){
		int y = deltaTime * object.second + object.first->getPos().y;
		int x = object.first->getPos().x;
		object.first->setPos({ x, y });
	}
}

void Game3::onStart(){
	Input::getInstance()->addListener(this);
}

void Game3::onStop(){
	Input::getInstance()->removeListener(this);
}

void Game3::onRender(Sprite* canvas){
}

void Game3::buttonPressed(uint i){
	if(i == BTN_BACK){
		pop();
	}
}

void Game3::addTemplate(std::string file, PixelDim dim, int value){
	Template temp{ file, dim, value };
	if(value > 0){
		foods.push_back(temp);
	}else{
		bombs.push_back(temp);
	}
}

void Game3::spawnRandom(){
	int randNum = rand() % (101);
	if(randNum <= 65){
		int pick = rand() % foods.size();
		spawnItem(foods[pick]);
	}else{
		int pick = rand() % bombs.size();
		spawnItem(bombs[pick]);
	}
}

void Game3::spawnItem(Game3::Template temp){
	int randPos = rand() % (161 - temp.dim.x - 10) + 10; //10 - so that it doesn't spawn on the bar
	auto go = std::make_shared<GameObject>(
			std::make_unique<StaticRC>(getFile(temp.path), temp.dim),
			std::make_unique<RectCC>(temp.dim)
	);
	addObject(go);
	float speed = rand() % (speedMax - speedMin + 1) + speedMin;

	movingObjects.insert(std::make_pair(go, speed));
	go->setPos({ randPos, -temp.dim.y });
	Item item{ go, temp.value };
	collision.addPair(*duck->getGameObject(), *item.go, [this, item, speed](){
		if(duck->isEatingBad()){
			collision.removePair(*duck->getGameObject(), *item.go);
			return;
		}

		movingObjects.erase(std::make_pair(item.go, speed));
		collisionHandler(item);
	});
	collision.addPair(*collectorBot, *item.go, [this, item, speed](){
		movingObjects.erase(std::make_pair(item.go, speed));
		removeObject(item.go);
	});
}

void Game3::collisionHandler(Item item){
	removeObject(item.go);
	duck->startEating(item.value);
	if(item.value > 0){
		hungerMeter += item.value;
		drawBar();
		if(hungerMeter >= hungerMeterMax){
			duck->filled(this);
		}
	}else{
		lives--;
		drawHearts();
	}
	if(lives <= 0){
		duck->killed(this);
		dead = true;
	}
}

void Game3::drawHearts(){
	hearts->clear(TFT_TRANSPARENT);
	for(int i = 0; i < lives; i++){
		hearts->drawIcon(getFile("/Heart.raw"), 0 + i * 8, 0, 7, 6);
	}
}


typedef struct {
	double r;       // a fraction between 0 and 1
	double g;       // a fraction between 0 and 1
	double b;       // a fraction between 0 and 1
} rgb;

typedef struct {
	double h;       // angle in degrees
	double s;       // a fraction between 0 and 1
	double v;       // a fraction between 0 and 1
} hsv;

static rgb hsv2rgb(hsv in);

rgb hsv2rgb(hsv in){
	double hh, p, q, t, ff;
	long i;
	rgb out;

	if(in.s <= 0.0){       // < is bogus, just shuts up warnings
		out.r = in.v;
		out.g = in.v;
		out.b = in.v;
		return out;
	}
	hh = in.h;
	if(hh >= 360.0) hh = 0.0;
	hh /= 60.0;
	i = (long) hh;
	ff = hh - i;
	p = in.v * (1.0 - in.s);
	q = in.v * (1.0 - (in.s * ff));
	t = in.v * (1.0 - (in.s * (1.0 - ff)));

	switch(i){
		case 0:
			out.r = in.v;
			out.g = t;
			out.b = p;
			break;
		case 1:
			out.r = q;
			out.g = in.v;
			out.b = p;
			break;
		case 2:
			out.r = p;
			out.g = in.v;
			out.b = t;
			break;

		case 3:
			out.r = p;
			out.g = q;
			out.b = in.v;
			break;
		case 4:
			out.r = t;
			out.g = p;
			out.b = in.v;
			break;
		case 5:
		default:
			out.r = in.v;
			out.g = p;
			out.b = q;
			break;
	}
	return out;
}


void Game3::drawBar(){
	float fillPercent = ((float) hungerMeter / (float) hungerMeterMax) * 121.0f;

	float difference = abs(118 - fillPercent);
	double hue = (118.0f - difference) / 100.0 * 60.0 / 255.0 * 360;
	rgb rgbColor0 = hsv2rgb({ hue, 1.0, 1.0 });
	uint16_t c0 = lgfx::color565(rgbColor0.r * 255.0, rgbColor0.g * 255.0, rgbColor0.b * 255.0);

	hungerBar->fillRect(2, 121 - fillPercent, 4, fillPercent, c0);
}

Stats Game3::returnStats(){
	return Stats({(uint8_t)(hungerMeter / 6), (uint8_t)(hungerMeter / 8),(uint8_t )(hungerMeter/10)});
}
