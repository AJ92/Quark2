#ifndef COMPONENTMANAGEMENT_H
#define COMPONENTMANAGEMENT_H

#include <algorithm>
#include <memory>
#include <unordered_map>
#include <vector>

#include "base/datastructures/components/component.h"

struct HashEnumCast {
    size_t operator()(Component::Type const& t) const {
        return (int) t;
    }
};

class ComponentManagement
{
public:
	ComponentManagement();
	~ComponentManagement();

	void addComponent(std::shared_ptr<Component> comp);
	void removeComponent(std::shared_ptr<Component> comp);

	std::vector<std::shared_ptr<Component> > & getAllComponentsByType(Component::Type type);

private:
	std::vector<std::shared_ptr<Component> > mEmptyCompVec;
	std::unordered_map<Component::Type, std::vector<std::shared_ptr<Component> >, HashEnumCast > mTypeCompMap;
};

#endif // COMPONENTMANAGEMENT_H
