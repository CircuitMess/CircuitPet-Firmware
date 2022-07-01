
#include <SPIFFS.h>
#include "Bar.h"

Bar::Bar(std::shared_ptr<Sprite> sprite, File barFile) : sprite(sprite), barFile(barFile){
	sprite->fillRect(0,63,9,3,TFT_BLACK);
}

int Bar::resetGoal(){
	srand((unsigned) time(NULL));
	yGoal = 14 + (rand() % 91);
	draw();
}

int Bar::getY(){
	return yGoal;
}

void Bar::draw(){
	sprite->drawIcon(barFile, 0, 0, 9, 120);
	sprite->drawRoundRect(0,0, 9, 120, TFT_BLACK);
	sprite->fillRect(0,yGoal, 9, 3, TFT_BLACK); //draw barGO
}
