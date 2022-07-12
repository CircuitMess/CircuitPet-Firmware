
#ifndef CIRCUITPET_FIRMWARE_GAME4_H
#define CIRCUITPET_FIRMWARE_GAME4_H


#include <Input/InputListener.h>
#include "../../GameEngine/Game.h"

class Game4 : public Game, private InputListener{
public:
	Game4();

protected:
	void onLoad() override;
	void onLoop(float deltaTime) override;
	void onStart() override;
	void onStop() override;
	void onRender(Sprite* canvas) override;

private:
	std::shared_ptr<GameObject> bg;
	std::shared_ptr<GameObject> duck;

};


#endif //CIRCUITPET_FIRMWARE_GAME4_H
