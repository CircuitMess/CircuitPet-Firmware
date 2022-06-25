#include <set>
#include <map>
#include "RenderSystem.h"

RenderSystem::RenderSystem(const std::vector<GameObject>& objects, Sprite* canvas) : GameSystem(objects), canvas(canvas){}

void RenderSystem::update(uint32_t deltaMicros){
	std::set<uint8_t> layers;
	std::map<uint8_t, std::vector<GameObject*>> layerMap;

	//map objects to layers
	for(auto object : objects){
		uint8_t layer = object.getRenderComponent()->getLayer();
		layers.insert(layer);
		layerMap[layer].push_back(&object);
	}

	std::vector<uint8_t> layersVec(layers.begin(), layers.end());
	std::sort(layersVec.begin(), layersVec.end());

	//iterate objects layer by layer
	for(auto layer : layersVec){
//		Serial.println(layer);
		for(auto object : layerMap[layer]){
			object->getRenderComponent()->push(canvas, object->getPos());
		}
	}
}
