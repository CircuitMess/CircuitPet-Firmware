#include <Pins.hpp>
#include "TestGame.h"
#include "../GameEngine/Rendering/AnimRC.h"
#include "../GameEngine/Rendering/StaticRC.h"
#include "../GameEngine/Collision/RectCC.h"
#include <Input/Input.h>

TestGame::TestGame() : Game("", {
		{ "/Pat1.gif", {}, true },
		{ "/Bg/Level1.raw", {}, true },
		{ "/MenuIcons/Icon1.raw", {}, true },
		{ "/MenuIcons/Icon2.raw", {}, true },
		{ "/MenuIcons/Icon3.raw", {}, true },
		{ "/MenuIcons/Icon4.raw", {}, true },
		{ "/MenuIcons/Icon5.raw", {}, true },
		{ "/MenuIcons/Icon6.raw", {}, true }
}){}

void TestGame::onLoad(){
	pat = std::make_shared<GameObject>(
			std::make_unique<AnimRC>(getFile("/Pat1.gif")),
			nullptr
	);
	addObject(pat);
	pat->setPos({ 50, 30 });

	bg = std::make_shared<GameObject>(
			std::make_unique<StaticRC>(getFile("/Bg/Level1.raw"), PixelDim { 160, 128 }),
			nullptr
	);
	addObject(bg);
	bg->getRenderComponent()->setLayer(-1);

	auto add = [this](std::string file, int16_t x, int16_t y){
		auto newIndex = objs.size();

		auto gObj = std::make_shared<GameObject>(
				std::make_unique<StaticRC>(getFile(file), PixelDim{ 32, 32 }),
				std::make_unique<RectCC>(glm::vec2 { 32, 32 })
		);

		Object obj { gObj, glm::vec2 {
				(((float) rand() / (float) RAND_MAX) - .5f) * 20.0f,
				(((float) rand() / (float) RAND_MAX) - .5f) * 20.0f
		} };

		obj.velocity.x += std::copysignf(20.0f, obj.velocity.x);
		obj.velocity.y += std::copysignf(20.0f, obj.velocity.y);

		gObj->setPos({ x, y });

		for(int i = 0; i < objs.size(); i++){
			collision.addPair(*objs[i].gObj, *gObj, [this, i, newIndex](){
				objs[i].velocity *= -1.0f;
				objs[newIndex].velocity *= -1.0f;
			});
		}

		collision.wallsHorizontal(*gObj, [this, newIndex](){
			objs[newIndex].velocity.y *= -1.0f;
		});

		collision.wallsVertical(*gObj, [this, newIndex](){
			objs[newIndex].velocity.x *= -1.0f;
		});

		addObject(gObj);
		objs.push_back(obj);
	};

	add("/MenuIcons/Icon1.raw", 10, 10);
	add("/MenuIcons/Icon2.raw", 50, 10);
	add("/MenuIcons/Icon3.raw", 100, 10);
	add("/MenuIcons/Icon4.raw", 15, 60);
	add("/MenuIcons/Icon5.raw", 55, 60);
	add("/MenuIcons/Icon6.raw", 120, 60);
}

void TestGame::onLoop(float deltaTime){
	for(int i = 0; i < objs.size(); i++){
		auto& obj = objs[i];
		obj.velocity += glm::vec2 { 0.0f, 40.0f } * deltaTime;
		obj.gObj->setPos(obj.gObj->getPos() + obj.velocity * deltaTime);
	}
}

void TestGame::onRender(Sprite* canvas){

}

void TestGame::onStart(){
	reinterpret_cast<AnimRC*>(pat->getRenderComponent().get())->start();
	Input::getInstance()->addListener(this);
}

void TestGame::onStop(){
	Input::getInstance()->removeListener(this);
}

void TestGame::buttonPressed(uint i){
	if(i == BTN_BACK){
		pop();
	}
}
