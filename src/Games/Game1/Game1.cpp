#include <Pins.hpp>
#include "Game1.h"
#include <Input/Input.h>
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
		{ "/Games/Game1/OilyIdle.gif",    {}, true },
		{ "/Games/Game1/OilyJump.gif",    {}, true },

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
	oilCanGO->setPos({ 105, 50 });
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

	auto scoreRc = std::make_unique<SpriteRC>(PixelDim{ 50, 7 });
	scoreSprite = scoreRc->getSprite();
	auto scoreGo = std::make_shared<GameObject>(
			move(scoreRc),
			nullptr
	);
	addObject(scoreGo);
	scoreGo->setPos({ 5, 5 });
	scoreSprite->clear(TFT_TRANSPARENT);
}

void Game1::onLoop(float deltaTime){
	indicator->move(deltaTime);
	if(oilCan->move(deltaTime)){
		duckGo->setPos({ 23, 16 }); //manually set for the gif to fit
		duckAnim->setAnim(getFile("/Games/Game1/OilyDone.gif"));
		duckAnim->setLoopDoneCallback([this](uint32_t i){
			delay(700);
			pop();
			return;
		});
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
	if(done) return;

	if(i == BTN_BACK){
		pop();
		return;
	}
	if(i == BTN_ENTER){
		tries++;
		addPoints(indicator->getDifference());
		scoreSprite->clear(TFT_TRANSPARENT);
		scoreSprite->setTextColor(TFT_WHITE);
		scoreSprite->setCursor(0, 0);
		scoreSprite->printf("Tries: %d", tries);
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
	indicator->setGoal(bar->getY());

	if(fillPercent >= 0.999f){
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
	}
}

