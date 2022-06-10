#include "ResourceManager.h"
#include <SPIFFS.h>
#include <FS/CompressedFile.h>
#include <FS/RamFile.h>

ResourceManager::ResourceManager(const char* root) : root(root){}

void ResourceManager::load(const std::vector<ResDescriptor>& descriptors){
	uint8_t copyBuffer[1024];

	for(const auto& descriptor : descriptors){
		if(descriptor.inRam){
			if(descriptor.compParams){

				//decode compressed file from SPIFFS to decoded RAMFile
				File original = CompressedFile::open(SPIFFS.open((root + descriptor.path).c_str()), descriptor.compParams.expansion,
													 descriptor.compParams.lookahead);
				File output = RamFile::open(nullptr, 0, false);

				while(size_t readBytes = original.read(copyBuffer, sizeof(copyBuffer))){
					output.write(copyBuffer, readBytes);
				}
				resources[descriptor.path.c_str()] = output;
			}else{
				//copy file from SPIFFS to RAMFile
				resources[descriptor.path.c_str()] = RamFile::open(SPIFFS.open((root + descriptor.path).c_str()));
			}

		}else{
			//use file from SPIFFS, not from RAM
			resources[descriptor.path.c_str()] = SPIFFS.open((root + descriptor.path).c_str());
		}
	}
}

File ResourceManager::getResource(const char* path){
	return resources[path];
}

