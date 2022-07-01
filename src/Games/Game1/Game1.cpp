
#include <Pins.hpp>
#include "Game1.h"
#include <Input/Input.h>
#include "../../GameEngine/Rendering/StaticRC.h"
#include "../../GameEngine/Rendering/SpriteRC.h"

Game1::Game1() : Game("/Games/Game1", {
		{ "/Arrow.raw", {}, true },
		{ "/Level0.raw", {}, true },
		{ "/BarEdge.raw", {}, true },
		{ "/OilyDone.gif", {}, false },
		{ "/OilyIdle.gif", {}, false },
		{ "/OilyJump.gif", {}, false }
}){}

void Game1::onLoad(){
	indi = std::make_shared<GameObject>(
			std::make_unique<StaticRC>(getFile("/Arrow.raw"), PixelDim{ 7, 11 }),
			nullptr
	);
	addObject(indi);
	indi->getRenderComponent()->setLayer(1);
	indi->setPos({ 140, 59 }); //59 = 64 - (11/2)
	indicator.setPointer(indi);


	/////////////////////////////////////////////////

//	std::unique_ptr<SpriteRC> spriterc = std::make_unique<SpriteRC>(PixelDim {10, 10});
//	customSprite = spriterc->getSprite();
//	customSprite->fillCircle(5, 5, 4, TFT_RED);
//	auto gobj = std::make_shared<GameObject>(std::move(spriterc), nullptr);
//	gobj->setPos({10, 10});

	//addObject(gobj);


	auto spriteRC = std::make_unique<SpriteRC>(PixelDim{9, 120});
	//spriteRC->getSprite()->fillRect(0, 63, 9, 3, TFT_BLACK);
	spriteRC->getSprite()->drawIcon(getFile("/Bar.raw"), 0 ,0, 9, 120);
	goal = std::make_shared<GameObject>(
			std::move(spriteRC),
			nullptr
	);
	addObject(goal);
	goal->getRenderComponent()->setLayer(1);
	goal->setPos({ 150, 4 });
	resetGoal();
	//////////////////////////////////////////////////////

	bg = std::make_shared<GameObject>(
			std::make_unique<StaticRC>(getFile("/Level0.raw"), PixelDim({ 160, 128 })),
			nullptr
	);
	addObject(bg);
	bg->getRenderComponent()->setLayer(0);

}

void Game1::onLoop(float deltaTime){
	indicator.move(deltaTime);
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
		if(tries < 0){pop();}

		//yPos is top of the indicator, +5 is the middle of the indicator
		addPoints(abs((indicator.getYPos() + 5) - (yGoal + 1)));
		resetGoal();
	}
}

void Game1::resetGoal(){
	srand((unsigned) time(NULL));
	yGoal = 14 + (rand() % 91);
	goal->setPos(PixelDim{ 150, yGoal});
}

void Game1::addPoints(int difference){
	int temp = 35 - 5*difference;
	if(temp <=0) return;
	fillPercent += (float)temp/100.0f;
	printf("fillPercent: %f\n", fillPercent);

	oilCan->fill(fillPercent);

	if(fillPercent >= 1.0f){
		printf("do the animation");
	}
}
