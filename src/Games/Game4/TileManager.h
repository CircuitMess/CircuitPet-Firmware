
#ifndef CIRCUITPET_FIRMWARE_TILEMANAGER_H
#define CIRCUITPET_FIRMWARE_TILEMANAGER_H


#include <memory>
#include <vector>
#include "../../GameEngine/GameObject.h"

class TileManager {
public:
	TileManager(std::vector<std::shared_ptr<GameObject>>& movingObjects);
	void addFilePair(File topFile, File botFile);
	void reset(int objectIndex);
	void create();

private:
	void drawTiles(int objectIndex);

	const int tileDim = 16;
	const int topY = 96;
	const int tilesPerArray = 5;
	const int arrays = 3;

	std::vector<File> topFiles;
	std::vector<File> botFiles;
	std::vector<std::shared_ptr<GameObject>>& movingObjects;
};


#endif //CIRCUITPET_FIRMWARE_TILEMANAGER_H
