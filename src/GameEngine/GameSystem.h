#ifndef CIRCUITPET_FIRMWARE_GAMESYSTEM_H
#define CIRCUITPET_FIRMWARE_GAMESYSTEM_H

#include "GameObject.h"
#include <vector>

class GameSystem {
public:
	GameSystem(const std::vector<GameObject>& objects);

	virtual void update(uint32_t deltaMicros) = 0;

protected:
	const std::vector<GameObject>& objects;
};


#endif //CIRCUITPET_FIRMWARE_GAMESYSTEM_H
