
#include "TileManager.h"
#include "../../GameEngine/Rendering/SpriteRC.h"
#include "../../GameEngine/Collision/RectCC.h"

TileManager::TileManager(std::vector<std::shared_ptr<GameObject>>& movingObjects) : movingObjects(movingObjects){
	srand(time(nullptr));
}

void TileManager::addFilePair(File topFile, File botFile){
	topFiles.push_back(topFile);
	botFiles.push_back(botFile);
}

void TileManager::create(){
	for(int iArray = 0; iArray < arrays; iArray++){
		auto setOfTiles = std::make_shared<GameObject>(
				std::make_unique<SpriteRC>(PixelDim{ tileDim * tilesPerArray, tileDim * 2 }),
				std::make_unique<RectCC>(glm::vec2{ tileDim, tileDim })
		);
		movingObjects.push_back(setOfTiles);
		drawTiles(iArray);
		setOfTiles->setPos({ iArray * tilesPerArray * tileDim, topY });
	}
}

void TileManager::reset(int objectIndex){
	drawTiles(objectIndex);
	movingObjects[objectIndex]->setPos({ (arrays - 1) * (tilesPerArray) * tileDim, topY });
}

void TileManager::drawTiles(int objectIndex){
	auto sprite = std::static_pointer_cast<SpriteRC>(movingObjects[objectIndex]->getRenderComponent())->getSprite();

	for(int iTile = 0; iTile < tilesPerArray; iTile++){
		int randTop = rand() % topFiles.size();
		int randBot = rand() % botFiles.size();
		sprite->drawIcon(topFiles[randTop], iTile * tileDim, 0, tileDim, tileDim);
		sprite->drawIcon(botFiles[randBot], iTile * tileDim, 16, tileDim, tileDim);
	}
}
