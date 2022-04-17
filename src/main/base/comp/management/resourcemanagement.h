#ifndef RESOURCEMANAGEMENT_H
#define RESOURCEMANAGEMENT_H

#include <algorithm>
#include <memory>
#include <unordered_map>
#include <vector>

#include "base/comp/icomponent.h"
#include "base/comp/scripting/script.h"

struct HashEnumCast {
    size_t operator()(IComponent::Type const& t) const {
        return (int) t;
    }
};

class ResourceManagement
{
public:
	ResourceManagement();
	~ResourceManagement();

	std::shared_ptr<IComponent> loadResource(std::string file, bool * success = nullptr);
	void freeResource();

	bool isLoaded(std::string file);

private:
	// file -> component mapping
	std::unordered_map<std::string, std::shared_ptr<IComponent> > mFileComponentMap;
	// file -> usage count mapping
	std::unordered_map<std::string, long > mFileUsageMap;
};

#endif // RESOURCEMANAGEMENT_H
