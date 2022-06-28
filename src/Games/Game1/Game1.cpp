
#include "Game1.h"

Game1::Game1() : Game("/srd/Games/Game1", {
		{"/arrow.raw", {}, true},
		{"/OilyDone.gif", {}, false},
		{"/OilyIdle.gif", {}, false},
		{"/OilyJump.gif", {}, false}
}), indicator((Sprite*) nullptr, 7, 11){}

void Game1::onStart(){

}

void Game1::onLoad(){

}

void Game1::onLoop(float deltaTime){

}

void Game1::onStop(){

}
