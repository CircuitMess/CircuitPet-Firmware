
#ifndef CIRCUITPET_FIRMWARE_GAMEIMAGE_H
#define CIRCUITPET_FIRMWARE_GAMEIMAGE_H

#include <Arduino.h>
#include <CircuitOS.h>
#include <Display/Color.h>
#include <FS.h>

class Sprite;

class GameImage {
public:
	GameImage();
	explicit GameImage(Sprite* canvas, const char* imgPath);
	GameImage(const GameImage& other);
	GameImage& operator=(const GameImage& other);
	virtual ~GameImage();

	explicit operator bool() const;

	void setCanvas(Sprite* canvas);

	int16_t getX() const;
	int16_t getY() const;
	void setX(int16_t x);
	void setY(int16_t y);

	void draw() const;

private:
	constexpr static uint8_t size = 32; //change this one for icons

	int16_t x;
	int16_t y;
	Color buffer[size * size];

	Sprite* canvas = nullptr;
};
#endif //CIRCUITPET_FIRMWARE_GAMEIMAGE_H
