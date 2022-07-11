#include <Pins.hpp>
#include "Game3.h"
#include "../../GameEngine/Rendering/SpriteRC.h"
#include "../../GameEngine/Rendering/StaticRC.h"
#include "../../GameEngine/Collision/RectCC.h"
#include <Input/Input.h>

Game3::Game3() : Game("/Games/Game3", {
		{ "/Background.raw", {}, true },
		{ "/Bomb.raw",       {}, true },
		{ "/Dynamite.raw",   {}, true },
		{ "/Nut.raw",        {}, true },
		{ "/Screw.raw",      {}, true },
		{ "/DuckWalk.gif",   {}, true },
		{ "/DuckEat.gif",    {}, true }
}){}

void Game3::onLoad(){
	bg = std::make_shared<GameObject>(
			std::make_unique<StaticRC>(getFile("/Background.raw"), PixelDim{ 160, 128 }),
			nullptr
	);
	addObject(bg);
	bg->getRenderComponent()->setLayer(-1);

	duck = new Duck(getFile("/DuckWalk.gif"), getFile("/DuckEat.gif"));
	addObject(duck->getGameObject());

	addTemplate("/Nut.raw", PixelDim{ 13, 13 }, 15);
	addTemplate("/Screw.raw", PixelDim{ 5, 15 }, 5);
	addTemplate("/Bomb.raw", PixelDim{ 14, 22 }, 0);
	addTemplate("/Dynamite.raw", PixelDim{ 5, 20 }, 0);

	items[0].go->setPos({70,50});
	items[1].go->setPos({60,50});
	items[2].go->setPos({30,60});
	items[3].go->setPos({20,60});


}

void Game3::onLoop(float deltaTime){
	duck->loop(deltaTime);
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

void Game3::collisionHandler(Item item){
	removeObject(item.go);
	duck->startEating();
	Serial.printf("collision happened %d \n", item.value);
}

