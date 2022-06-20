#include "ResourceManager.h"
#include <SPIFFS.h>
#include <FS/CompressedFile.h>
#include <FS/RamFile.h>

ResourceManager::ResourceManager(const char* root) : root(root){}

void ResourceManager::load(const std::vector<ResDescriptor>& descriptors){
	uint8_t copyBuffer[1024];

	for(const auto& descriptor : descriptors){

		std::string path = root + descriptor.path;
		auto cPath = path.c_str();

		if(descriptor.inRam){
			if(descriptor.compParams){

				//decode compressed file from SPIFFS to decoded RAMFile
				File original = CompressedFile::open(SPIFFS.open(cPath), descriptor.compParams.expansion,
													 descriptor.compParams.lookahead);
				File output = RamFile::open(nullptr, 0, false);

				while(size_t readBytes = original.read(copyBuffer, sizeof(copyBuffer))){
					output.write(copyBuffer, readBytes);
				}
				resources[descriptor.path] = output;
			}else{
				//copy file from SPIFFS to RAMFile
				resources[descriptor.path] = RamFile::open(SPIFFS.open(cPath));
			}

		}else{
			//use file from SPIFFS, not from RAM
			resources[descriptor.path] = SPIFFS.open(cPath);
		}
	}
}

File ResourceManager::getResource(std::string path){
	if(resources.find(path) != resources.end()) return File();

	return resources[path];
}

