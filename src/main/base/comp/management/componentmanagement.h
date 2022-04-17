#ifndef COMPONENTMANAGEMENT_H
#define COMPONENTMANAGEMENT_H

#include <algorithm>
#include <memory>
#include <unordered_map>
#include <vector>

#include "base/comp/icomponent.h"

struct HashEnumCast {
    size_t operator()(IComponent::Type const& t) const {
        return (int) t;
    }
};

class ComponentManagement
{
public:
	ComponentManagement();
	~ComponentManagement();

	void addComponent(std::shared_ptr<IComponent> comp);
	void removeComponent(std::shared_ptr<IComponent> comp);

	std::vector<std::shared_ptr<IComponent> > & getAllComponentsByType(IComponent::Type type);

private:
	std::vector<std::shared_ptr<IComponent> > mEmptyCompVec;
	std::unordered_map<IComponent::Type, std::vector<std::shared_ptr<IComponent> >, HashEnumCast > mTypeCompMap;
};

#endif // COMPONENTMANAGEMENT_H
