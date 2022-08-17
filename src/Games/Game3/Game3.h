
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
	Stats returnStats() override;

private:
	void buttonPressed(uint i) override;
	void addTemplate(std::string file, PixelDim dim, int value);
	void spawnRandom();
	void drawHearts();
	void drawBar();

	struct Item {
		std::shared_ptr<GameObject> go;
		int value;
	};

	struct Template {
		std::string path;
		PixelDim dim;
		int value;
	};

	void spawnItem(Template temp);
	void collisionHandler(Item item);

	const int hungerMeterMax = 200;
	int hungerMeter = 0;
	int lives = 3;
	const int speedMin = 60;
	const int speedMax = 80;
	const float spawnRate = 0.5f;
	float timeToSpawn = 0.0f;
	std::unique_ptr<Duck> duck;

	std::shared_ptr<Sprite> hearts;
	std::shared_ptr<Sprite> hungerBar;
	std::shared_ptr<GameObject> bg;
	std::shared_ptr<GameObject> collectorBot;
	std::set<std::pair<std::shared_ptr<GameObject>, float>> movingObjects;
	std::vector<Template> foods;
	std::vector<Template> bombs;
};


#endif //CIRCUITPET_FIRMWARE_GAME3_H
