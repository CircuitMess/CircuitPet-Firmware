#include <Arduino.h>
#include "src/GameEngine/Game.h"
#include <Loop/LoopListener.h>
#include "src/GameEngine/Rendering/RenderSystem.h"
#include "src/GameEngine/Collision/CollisionSystem.h"
#include "src/GameEngine/Collision/SquareCC.h"
#include "src/GameEngine/Rendering/AnimRC.h"
#include <Loop/LoopManager.h>

class MyGame : public Game, private LoopListener {
public:
	MyGame() : Game("/", {{ "big1.gif", { 8, 4 }, true },
						  { "big2.gif", { 8, 4 }, true }}),
						  renderer(){
	}

	void loop(uint micros) override{

	}
protected:
	void onLoad() override{
		Serial.println("loaded");
	}

	void onStart() override{

		addObject(std::make_shared<GameObject>(std::unique_ptr<AnimRC>(new AnimRC(getFile("big1.gif"))),
		        std::unique_ptr<SquareCC>(new SquareCC({8, 9}))));
		LoopManager::addListener(this);
	}

	void onStop() override{
		LoopManager::removeListener(this);
	}

private:
	RenderSystem renderer;
	CollisionSystem collisions;
};

void setup(){

}

void loop(){

}