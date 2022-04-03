#ifndef RESOURCEMANAGEMENT_H
#define RESOURCEMANAGEMENT_H

#include <algorithm>
#include <memory>
#include <unordered_map>
#include <vector>

#include "base/datastructures/components/component.h"
#include "base/datastructures/components/script.h"

struct HashEnumCast {
    size_t operator()(Component::Type const& t) const {
        return (int) t;
    }
};

class ResourceManagement
{
public:
	ResourceManagement();
	~ResourceManagement();

	std::shared_ptr<Component> loadResource(std::string file, bool * success = nullptr);
	void freeResource(Component comp);

	bool isLoaded(std::string file);

private:
	// file -> component mapping
	std::unordered_map<std::string, std::shared_ptr<Component> > mFileComponentMap;
	// file -> usage count mapping
	std::unordered_map<std::string, long > mFileUsageMap;
};

#endif // RESOURCEMANAGEMENT_H
