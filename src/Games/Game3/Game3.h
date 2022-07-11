
#ifndef CIRCUITPET_FIRMWARE_GAME3_H
#define CIRCUITPET_FIRMWARE_GAME3_H


#include <Input/InputListener.h>
#include "../../GameEngine/Game.h"
#include "Duck.h"

class Game3 : public Game, private InputListener {
public:
	Game3();

protected:
	void onLoad() override;
	void onLoop(float deltaTime) override;
	void onStart() override;
	void onStop() override;
	void onRender(Sprite* canvas) override;

private:
	void buttonPressed(uint i) override;
	void addTemplate(std::string file, PixelDim dim, int value);
	void spawnRandom();

	struct Item{
		std::shared_ptr<GameObject> go;
		int value;
	};

	struct Template{
		std::string path;
		PixelDim dim;
		int value;
	};
	void spawnItem(Template temp);
	void collisionHandler(Item item);

	int hugerMeter = 200;
	int lives = 3;
	const float fallSpeed = 30.0f;
	const float spawnRate = 1.5f;
	float timeToSpawn = 0.0f;

	Duck* duck;
	std::shared_ptr<GameObject> bg;
	std::vector<Item> items;
	std::vector<Template> foods;
	std::vector<Template> bombs;
};


#endif //CIRCUITPET_FIRMWARE_GAME3_H
