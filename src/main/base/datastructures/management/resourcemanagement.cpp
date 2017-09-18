#include "resourcemanagement.h"
#include <iostream>

//little helper
bool hasEnding (std::string const &fullString, std::string const &ending) {
    if (fullString.length() >= ending.length()) {
        return (0 == fullString.compare (fullString.length() - ending.length(), ending.length(), ending));
    }
	return false;
}


ResourceManagement::ResourceManagement() {

}

ResourceManagement::~ResourceManagement() {

}

std::shared_ptr<Component> ResourceManagement::loadResource(std::string file, bool * success) {
	//check if already loaded
	if(isLoaded(file)){
		auto it = _file_component_map.find(file);
		if(success != nullptr){
			*success = true;
		}
		return it->second;
	}
	
	if(hasEnding(file, ".jpg")){
		//Texture
	}

	if(success != nullptr){
		*success = false;
	}
	return std::make_shared<Script>();
}

void ResourceManagement::freeResource(Component comp) {
	return;
}

bool ResourceManagement::isLoaded(std::string file){
	auto it = _file_component_map.find(file);
	if(it != _file_component_map.end()){
		return true;
	}
	return false;
}

