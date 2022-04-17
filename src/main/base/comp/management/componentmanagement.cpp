#include "componentmanagement.h"
#include <iostream>


ComponentManagement::ComponentManagement() {

}

ComponentManagement::~ComponentManagement() {

}

void ComponentManagement::addComponent(std::shared_ptr<IComponent> comp) {
	auto it_map = mTypeCompMap.find(comp->getType());

	if (it_map == mTypeCompMap.end()) {
		//create a new vector
		mTypeCompMap[comp->getType()] = std::vector<std::shared_ptr<IComponent> >();
		it_map = mTypeCompMap.find(comp->getType());
	}

	it_map->second.push_back(comp);
}

void ComponentManagement::removeComponent(std::shared_ptr<IComponent> comp) {
	auto it_map = mTypeCompMap.find(comp->getType());

	if (it_map == mTypeCompMap.end()) {
		//no vector of this type created yet so we don't have
		//any comps of this type yet!!!
		return;
	}

	auto it_vec = std::find(it_map->second.begin(), it_map->second.end(), comp);

	if (it_vec != it_map->second.end()) {
		//delete the found element from the vector
		it_map->second.erase(it_vec);
	}
}

std::vector<std::shared_ptr<IComponent> > & ComponentManagement::getAllComponentsByType(
	IComponent::Type type)
{
	auto it_map = mTypeCompMap.find(type);

	if (it_map == mTypeCompMap.end()) {

		return mEmptyCompVec;
	}

	return it_map->second;
}
