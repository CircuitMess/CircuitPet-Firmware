
#include <Pins.hpp>
#include "Game1.h"
#include <Input/Input.h>
#include "../../GameEngine/Rendering/StaticRC.h"
#include "../../GameEngine/Rendering/SpriteRC.h"

Game1::Game1() : Game("/Games/Game1", {
		{ "/Arrow.raw",    {}, true },
		{ "/Level0.raw",   {}, true },
		{ "/BarEdge.raw",  {}, true },
		{ "/Bar.raw",      {}, true },
		{ "/OilyDone.gif", {}, false },
		{ "/OilyIdle.gif", {}, false },
		{ "/OilyJump.gif", {}, false }
}){}

void Game1::onLoad(){
	indicatorGO = std::make_shared<GameObject>(
			std::make_unique<StaticRC>(getFile("/Arrow.raw"), PixelDim{ 7, 11 }),
			nullptr
	);
	addObject(indicatorGO);
	indicatorGO->getRenderComponent()->setLayer(1);
	indicatorGO->setPos({ 140, 59 }); //59 = 64 - (11/2)
	indicator = new Indicator(indicatorGO);



	/////////////////////////////////////////////////

//	std::unique_ptr<SpriteRC> spriterc = std::make_unique<SpriteRC>(PixelDim {10, 10});
//	customSprite = spriterc->getSprite();
//	customSprite->fillCircle(5, 5, 4, TFT_RED);
//	auto gobj = std::make_shared<GameObject>(std::move(spriterc), nullptr);
//	gobj->setPos({10, 10});

	//addObject(gobj);

	/*
	auto rcBar = std::make_unique<SpriteRC>(PixelDim{10, 90});
	auto barSprite = rcBar->getSprite();
	bar = std::make_shared<GameObject>(std::move(rcBar), nullptr);
	bar->setPos()
	addObject(bar)
	*/

	//////////////////////////////////////////////////////
	auto spriteCan = std::make_unique<SpriteRC>(PixelDim{ 10, 15 });
	oilCan = new OilCan(spriteCan->getSprite());
	oilCanGO = std::make_shared<GameObject>(
			std::move(spriteCan),
			nullptr
	);
	addObject(oilCanGO);
	oilCanGO->setPos({ 100, 60 });



	auto spriteBar = std::make_unique<SpriteRC>(PixelDim{ 9, 120 });
	bar = new Bar(spriteBar->getSprite(), getFile("/Bar.raw"));
	bar->resetGoal();
	barGO = std::make_shared<GameObject>(
			std::move(spriteBar),
			nullptr
	);
	addObject(barGO);
	barGO->getRenderComponent()->setLayer(1);
	barGO->setPos({ 150, 4 });


	bg = std::make_shared<GameObject>(
			std::make_unique<StaticRC>(getFile("/Level0.raw"), PixelDim({ 160, 128 })),
			nullptr
	);
	addObject(bg);
	bg->getRenderComponent()->setLayer(0);

}

void Game1::onLoop(float deltaTime){
	indicator->move(deltaTime);
}

void Game1::onRender(Sprite* canvas){

}

void Game1::onStart(){
	Input::getInstance()->addListener(this);
}

void Game1::onStop(){
	Input::getInstance()->removeListener(this);
}

void Game1::buttonPressed(uint i){
	if(i == BTN_BACK){
		pop();
	}
	if(i == BTN_ENTER){
		tries--;
		if(tries < 0){ pop(); }

		//yPos is top of the indicator, +5 is the middle of the indicator
		addPoints(abs((indicator->getYPos() + 5) - (bar->getY() + 1)));
		bar->resetGoal();
	}
}

void Game1::resetGoal(){
	srand((unsigned) time(NULL));
	yGoal = 14 + (rand() % 91);
	goal->setPos(PixelDim{ 150, yGoal});
}

void Game1::addPoints(int difference){
	int temp = 35 - 5 * difference;
	if(temp <= 0) return;
	fillPercent += (float) temp / 100.0f;

	oilCan->fill(fillPercent);

	if(fillPercent >= 1.0f){
		printf("do the animation");
	}
}
