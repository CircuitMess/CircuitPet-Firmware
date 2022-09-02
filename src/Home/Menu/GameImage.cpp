#include "GameImage.h"
#include <Display/Sprite.h>
#include <Display/Color.h>
#include <SPIFFS.h>

GameImage::GameImage(){}

GameImage::GameImage(Sprite* canvas, const char* imgPath) : canvas(canvas){
	fs::File itemFile = SPIFFS.open(imgPath);
	if(!itemFile) printf("failed\n");
	itemFile.read((uint8_t *)buffer, size*size*2);
}

GameImage::GameImage(const GameImage& other){
	memcpy(buffer, other.buffer, size * size * 2);
	this->canvas = other.canvas;
}

GameImage& GameImage::operator=(const GameImage& other){
	if(&other == this) return *this;

	this->canvas = other.canvas;
	free(buffer);
	memcpy(buffer, other.buffer, size * size * 2);
	return *this;
}

GameImage::~GameImage(){
}

GameImage::operator bool() const{
	return canvas != nullptr && buffer != nullptr;
}

void GameImage::draw() const {
	if(!*this) return;
	canvas->drawIcon(buffer, x, y, size, size, 1, TFT_TRANSPARENT);
}

int16_t GameImage::getX() const{
	return x;
}

void GameImage::setX(int16_t x){
	GameImage::x = x;
}

int16_t GameImage::getY() const{
	return y;
}

void GameImage::setY(int16_t y){
	GameImage::y = y;
}

void GameImage::setCanvas(Sprite* canvas){
	GameImage::canvas = canvas;
}