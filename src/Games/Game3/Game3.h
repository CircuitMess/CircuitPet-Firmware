
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
	void addItem(std::string file, bool edible, int value, PixelDim dim);

	struct Item{
		std::shared_ptr<GameObject> go;
		bool edible;
		float fallSpeed; //will be changed via spawner
		int value; 		 //used not only for score, but for the rarity of the spawns
	};
	void collisionHandler(Item item);


	Duck* duck;
	std::shared_ptr<GameObject> bg;
	std::vector<Item> items;
};


#endif //CIRCUITPET_FIRMWARE_GAME3_H
