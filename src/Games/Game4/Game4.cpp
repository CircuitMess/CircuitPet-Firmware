
#include "Game4.h"
#include "../../GameEngine/Rendering/StaticRC.h"

Game4::Game4() : Game("/Games/Game4",{
		{"/Background.raw", {}, true},
		{"/TileUp1.raw", {}, true},
		{"/TileUp2.raw", {}, true},
		{"/TileDown1.raw", {}, true},
		{"/TileDown2.raw", {}, true},
		{"/ObjectJump1.raw", {}, true},
		{"/ObjectJump2.raw", {}, true},
		{"/ObjectJump3.raw", {}, true},
		{"/ObjectJump4.raw", {}, true},
		{"/ObjectDuck1.raw", {}, true},
		{"/ObjectDuck2.raw", {}, true},
		{"/ObjectDuck3.raw", {}, true}
}){}

void Game4::onLoad(){
	bg = std::make_shared<GameObject>(
		std::make_unique<StaticRC>(getFile("/Background.raw"), PixelDim {160, 128}),
		nullptr
	);
	addObject(bg);

}

void Game4::onLoop(float deltaTime){
	Game::onLoop(deltaTime);
}

void Game4::onStart(){
	Game::onStart();
}

void Game4::onStop(){
	Game::onStop();
}

void Game4::onRender(Sprite* canvas){
	Game::onRender(canvas);
}
