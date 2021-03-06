#ifndef MODELLOADER_H
#define MODELLOADER_H

#include <algorithm>
#include <memory>
#include <unordered_map>
#include <vector>

#include "base/datastructures/components/component.h"
#include "base/datastructures/components/script.h"


class ModelLoader
{
	
public:
	ModelLoader();
	~ModelLoader();

	/*

	std::shared_ptr<Component> loadResource(std::string file, bool * success = nullptr);
	void freeResource(Component comp);

	bool isLoaded(std::string file);

private:
	// file -> component mapping
	std::unordered_map<std::string, std::shared_ptr<Component> > _file_component_map;
	// file -> usage count mapping
	std::unordered_map<std::string, long > _file_usage_map;
	*/
};

#endif // MODELLOADER_H
