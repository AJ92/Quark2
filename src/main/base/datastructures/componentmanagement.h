#ifndef COMPONENTMANAGEMENT_H
#define COMPONENTMANAGEMENT_H

#include <algorithm>
#include <memory>
#include <map>
#include <vector>

#include "base/datastructures/components/component.h"

class ComponentManagement
{
public:
	ComponentManagement();
	~ComponentManagement();

	void addComponent(std::shared_ptr<Component> comp);
	void removeComponent(std::shared_ptr<Component> comp);

	std::vector<std::shared_ptr<Component> > getAllComponentsByType(Component::Type type);

private:
	std::map<Component::Type, std::vector<std::shared_ptr<Component> > > _type_component_map;
};

#endif // COMPONENTMANAGEMENT_H
