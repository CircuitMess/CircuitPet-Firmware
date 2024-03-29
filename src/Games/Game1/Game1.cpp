#include <Pins.hpp>
#include "Game1.h"
#include <Input/Input.h>
#include <CircuitPet.h>
#include "../../GameEngine/Rendering/StaticRC.h"
#include "../../GameEngine/Rendering/SpriteRC.h"
#include "../../Stats/StatsManager.h"

std::string bgPath[6] = { "/Bg/Level1.raw",
						  "/Bg/Level2.raw",
						  "/Bg/Level3.raw",
						  "/Bg/Level4.raw",
						  "/Bg/Level5.raw",
						  "/Bg/Level6.raw"
};

Game1::Game1() : Game("", {
		{ "/Games/Game1/Arrow.raw",       {}, true },
		{ "/Games/Game1/EmptyCan.raw",    {}, true },
		{ bgPath[StatMan.getLevel() - 1], {}, true },
		{ "/Games/Game1/FullCan.raw",     {}, true },
		{ "/Games/Game1/OilyDone.gif",    {}, false },
		{ "/Games/Game1/OilyIdle.gif",    {}, false },
		{ "/Games/Game1/OilyJump.gif",    {}, false },

}){}

void Game1::onLoad(){
	auto spriteBar = std::make_unique<SpriteRC>(PixelDim{ 9, 120 });
	bar = std::make_unique<Bar>(spriteBar->getSprite());
	bar->resetGoal();
	barGO = std::make_shared<GameObject>(
			std::move(spriteBar),
			nullptr
	);
	addObject(barGO);
	barGO->getRenderComponent()->setLayer(1);
	barGO->setPos({ 150, 4 });

	indicatorGO = std::make_shared<GameObject>(
			std::make_unique<StaticRC>(getFile("/Games/Game1/Arrow.raw"), PixelDim{ 7, 11 }),
			nullptr
	);
	addObject(indicatorGO);
	indicatorGO->getRenderComponent()->setLayer(1);
	indicator = std::make_unique<Indicator>(indicatorGO);
	indicator->setGoal(bar->getY());

	auto spriteCan = std::make_unique<SpriteRC>(PixelDim{ 24, 21 });
	oilCan = std::make_unique<OilCan>(spriteCan->getSprite(), getFile("/Games/Game1/FullCan.raw"), getFile("/Games/Game1/EmptyCan.raw"));
	oilCanGO = std::make_shared<GameObject>(
			std::move(spriteCan),
			nullptr
	);
	addObject(oilCanGO);
	oilCanGO->setPos({ 105, 40 });
	oilCanGO->getRenderComponent()->setLayer(1);
	oilCan->setGameObject(oilCanGO);

	duckGo = std::make_shared<GameObject>(
			std::make_unique<AnimRC>(getFile("/Games/Game1/OilyIdle.gif")),
			nullptr
	);
	addObject(duckGo);
	duckGo->getRenderComponent()->setLayer(1);
	duckGo->setPos({ 55, 30 });
	duckAnim = std::static_pointer_cast<AnimRC>(duckGo->getRenderComponent());

	bg = std::make_shared<GameObject>(
			std::make_unique<StaticRC>(getFile(bgPath[StatMan.getLevel() - 1]), PixelDim({ 160, 128 })),
			nullptr
	);
	addObject(bg);
	bg->getRenderComponent()->setLayer(0);
}

void Game1::onLoop(float deltaTime){
	indicator->move(deltaTime);
	if(oilCan->move(deltaTime)){
		duckGo->setPos({ 23, 39 }); //manually set for the gif to fit
		duckAnim->setAnim(getFile("/Games/Game1/OilyDone.gif"));
		duckAnim->setLoopDoneCallback([this](uint32_t i){
			delay(700);
			pop();
			return;
		});
	};
}

void Game1::onRender(Sprite* canvas){
	auto base = CircuitPet.getDisplay()->getBaseSprite();
	if(StatMan.getLevel() <= 2){
		base->setTextColor(TFT_WHITE);
	}else if(StatMan.getLevel() <= 4){
		base->setTextColor(TFT_BLACK);
	}else{
		base->setTextColor(TFT_WHITE);
	}

	base->setTextFont(0);
	base->setTextSize(0);
	base->setCursor(1, 2);
	base->printf("Tries: %d", tries);
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
	if(done) return;

	if(i == BTN_BACK){
		Audio.play(Sound { Chirp { 400, 350, 50 }});
		pop();
		return;
	}
	if(i == BTN_LEFT){
		tries++;
		RGB.blinkTwice(bar->getColor(indicator->getDifference()));
		addPoints(indicator->getDifference());
	}
}

void Game1::resetAnim(){
	duckAnim->setAnim(getFile("/Games/Game1/OilyIdle.gif"));
	duckAnim->setLoopDoneCallback({});
}

void Game1::addPoints(int difference){
	multiplier = (length - (float) difference) / length;
	multiplier = pow(multiplier, 12);
	fillPercent += multiplier * maxPoints;
	oilCan->fill(fillPercent);
	bar->resetGoal();

	if(done){

	}else if(indicator->getDifference() < 30){
		Audio.play({{250, 200, 50}, {400, 700, 50}});
	}else if(indicator->getDifference() >= 30){
		Audio.play({{300, 300, 50}, {0, 0, 50}, {300, 300, 50}});
	}

	if(fillPercent >= 0.999f){
		Sound s = {{ 600, 400 , 200 }, { 400, 1000 , 200 }};
		Audio.play(s);

		Input::getInstance()->removeListener(this);
		removeObject(barGO);
		removeObject(indicatorGO);
		oilCan->startMoving();
		done = true;
	}else{
		duckAnim->setAnim(getFile("/Games/Game1/OilyJump.gif"));
		duckAnim->setLoopDoneCallback([this](uint32_t){
			resetAnim();
		});

		if(indicator->getDifference() < 30){
			Audio.play({{250, 200, 50}, {400, 700, 50}});
		}else if(indicator->getDifference() >= 30){
			Audio.play({{300, 300, 50}, {0, 0, 50}, {300, 300, 50}});
		}
	}

	indicator->setGoal(bar->getY());
}

Stats Game1::returnStats(){
	if(!done) return Game::returnStats();

	float success = (float)(minTries) /(float)(tries); // 0 - 1.0
	return Stats({ (uint8_t)(25.0*success), (uint8_t)(60.0*success), (uint16_t)(50.0*success) });
}
