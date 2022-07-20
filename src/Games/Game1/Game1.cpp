
#include <Pins.hpp>
#include "Game1.h"
#include <Input/Input.h>
#include "../../GameEngine/Rendering/StaticRC.h"
#include "../../GameEngine/Rendering/SpriteRC.h"

Game1::Game1() : Game("/Games/Game1", {
		{ "/Arrow.raw",      {}, true },
		{ "/Background.raw", {}, true },
		{ "/EmptyCan.raw",   {}, true },
		{ "/FullCan.raw",    {}, true },
		{ "/Hearth.raw",     {}, true },
		{ "/OilyDone.gif",   {}, false },
		{ "/OilyIdle.gif",   {}, true },
		{ "/OilyJump.gif",   {}, true }
}){}

void Game1::onLoad(){
	auto spriteBar = std::make_unique<SpriteRC>(PixelDim{ 9, 120 });
	bar = new Bar(spriteBar->getSprite());
	bar->resetGoal();
	barGO = std::make_shared<GameObject>(
			std::move(spriteBar),
			nullptr
	);
	addObject(barGO);
	barGO->getRenderComponent()->setLayer(1);
	barGO->setPos({ 150, 4 });

	indicatorGO = std::make_shared<GameObject>(
			std::make_unique<StaticRC>(getFile("/Arrow.raw"), PixelDim{ 7, 11 }),
			nullptr
	);
	addObject(indicatorGO);
	indicatorGO->getRenderComponent()->setLayer(1);
	indicator = new Indicator(indicatorGO);
	indicator->setGoal(bar->getY());

	auto spriteCan = std::make_unique<SpriteRC>(PixelDim{ 24, 21 });
	oilCan = new OilCan(spriteCan->getSprite(), getFile("/FullCan.raw"), getFile("/EmptyCan.raw"));
	oilCanGO = std::make_shared<GameObject>(
			std::move(spriteCan),
			nullptr
	);
	addObject(oilCanGO);
	oilCanGO->setPos({ 99, 60 });
	oilCanGO->getRenderComponent()->setLayer(1);
	oilCan->setGameObject(oilCanGO);

	duckGo = std::make_shared<GameObject>(
			std::make_unique<AnimRC>(getFile("/OilyIdle.gif")),
			nullptr
	);
	addObject(duckGo);
	duckGo->getRenderComponent()->setLayer(1);
	duckGo->setPos({ 50, 30 });
	duckAnim = std::static_pointer_cast<AnimRC>(duckGo->getRenderComponent());

	bg = std::make_shared<GameObject>(
			std::make_unique<StaticRC>(getFile("/Background.raw"), PixelDim({ 160, 128 })),
			nullptr
	);
	addObject(bg);
	bg->getRenderComponent()->setLayer(0);

	auto heartsSprite =	std::make_unique<SpriteRC>(PixelDim{48,6});
	hearts = heartsSprite->getSprite();
	auto heartsGO = std::make_shared<GameObject>(
			move(heartsSprite),
			nullptr
	);
	addObject(heartsGO);
	heartsGO->setPos({5 ,5});

	drawHearts();
}

void Game1::onLoop(float deltaTime){
	indicator->move(deltaTime);
	if(oilCan->move(deltaTime)){
		duckGo->setPos({ 17, 16 }); //manually set for the gif to fit
		duckAnim->setAnim(getFile("/OilyDone.gif"));
		duckAnim->setLoopDoneCallback([this](uint32_t i){
			printf("popped\n");
			pop();});
	};
}

void Game1::onRender(Sprite* canvas){

}

void Game1::onStart(){
	Input::getInstance()->addListener(this);
	duckAnim->start();
}

void Game1::onStop(){
	Input::getInstance()->removeListener(this);
	duckAnim->stop();
}

void Game1::buttonPressed(uint i){
	if(i == BTN_BACK){
		pop();
	}
	if(i == BTN_ENTER){
		tries--;
		drawHearts();
		if(tries < 0){ pop(); }
		addPoints(indicator->getDifference());
	}
}

void Game1::resetAnim(){
	duckAnim->setAnim(getFile("/OilyIdle.gif"));
	duckAnim->setLoopDoneCallback({});
}

void Game1::addPoints(int difference){
	multiplier = (length - (float) difference) / length;
	multiplier = pow(multiplier, 4);
	fillPercent += multiplier * maxPoints;
	oilCan->fill(fillPercent);
	Serial.printf("multi: %f, diff: %d\n", multiplier, difference);
	Serial.printf("FillPRCNT: %f\n", fillPercent);
	bar->resetGoal();
	indicator->setGoal(bar->getY());

	if(fillPercent >= 0.999f){
		Input::getInstance()->removeListener(this);
		removeObject(barGO);
		removeObject(indicatorGO);
		delete (indicator);
		delete (bar);
		oilCan->startMoving();
	}else{
		duckAnim->setAnim(getFile("/OilyJump.gif"));
		duckAnim->setLoopDoneCallback([this](uint32_t){
			resetAnim();
		});
	}
}

void Game1::drawHearts(){
	hearts->clear(TFT_TRANSPARENT);
	for(int i = 0; i < tries; i++){
		hearts->drawIcon(getFile("/Hearth.raw"),0 + i*8,0,7,6);
	}
}
