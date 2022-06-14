#ifndef CIRCUITPET_FIRMWARE_RESOURCEMANAGER_H
#define CIRCUITPET_FIRMWARE_RESOURCEMANAGER_H

#include <Arduino.h>
#include <FS.h>
#include <map>

struct CompParams {
	uint8_t lookahead;
	uint8_t expansion;

	explicit operator bool() const{
		return !lookahead || !expansion;
	};
};

struct ResDescriptor {
	std::string path;
	CompParams compParams;
	bool inRam;
};

class ResourceManager {
public:
	ResourceManager(const char* root);
	void load(const std::vector<ResDescriptor>& descriptors);

	File getResource(std::string path);

private:
	std::map<std::string, File> resources; //RamFiles
	const char* root;
};


#endif //CIRCUITPET_FIRMWARE_RESOURCEMANAGER_H
