
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

	struct Item{
		std::shared_ptr<GameObject> go;
//		bool edible;
//		float fallSpeed; //will be changed via spawner
		int value;
	};

	struct Template{
		std::string path;
		PixelDim dim;
		int value;
	};
	void collisionHandler(Item item);

	int hugerMeter = 200;
	int lives = 3;

	Duck* duck;
	std::shared_ptr<GameObject> bg;
	std::vector<Item> items;
	std::vector<Template> foods;
	std::vector<Template> bombs;
};


#endif //CIRCUITPET_FIRMWARE_GAME3_H
