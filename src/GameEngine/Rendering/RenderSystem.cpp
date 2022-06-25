#include <set>
#include <map>
#include "RenderSystem.h"

RenderSystem::RenderSystem(const Game* game, Sprite* canvas) : GameSystem(game), canvas(canvas){}

void RenderSystem::update(uint32_t deltaMicros){
	std::set<uint8_t> layers;
	std::map<uint8_t, std::vector<std::shared_ptr<const GameObject>>> layerMap;


	//map objects to layers
	for(const auto& object : getObjects()){
		auto rc = object->getRenderComponent();
		if(!rc) continue;

		uint8_t layer = rc->getLayer();
		layers.insert(layer);
		layerMap[layer].push_back(object);

	}

	std::vector<uint8_t> layersVec(layers.begin(), layers.end());
	std::sort(layersVec.begin(), layersVec.end());

	//iterate objects layer by layer
	for(auto layer : layersVec){
//		Serial.println(layer);
		for(const auto& object : layerMap[layer]){
			object->getRenderComponent()->push(canvas, object->getPos());
		}
	}
}
