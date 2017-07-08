#include "entitymanagement.h"
#include <iostream>


EntityManagement::EntityManagement() {

}

EntityManagement::~EntityManagement() {

}

std::vector<std::shared_ptr<Component> > EntityManagement::getAllComponentsFromEntity(
	std::shared_ptr<Entity> entity)
{
	std::map<std::string, std::vector<std::shared_ptr<Component> > >::iterator it_map;
	it_map = _identifier_component_list_map.find(entity->getIdentifier());

	if (it_map == _identifier_component_list_map.end()) {
		return std::vector<std::shared_ptr<Component> >();
	}

	return it_map->second;
}

std::vector<std::shared_ptr<Component> > EntityManagement::getAllComponentsFromEntityByType(
	std::shared_ptr<Entity> entity, Component::Type type)
{
	std::vector<std::shared_ptr<Component> > result;

	std::map<std::string, std::vector<std::shared_ptr<Component> > >::iterator it_map;
	it_map = _identifier_component_list_map.find(entity->getIdentifier());

	if (it_map == _identifier_component_list_map.end()) {
		return result;
	}

	for (std::vector<std::shared_ptr<Component> >::iterator it = it_map->second.begin();
		it != it_map->second.end();
		++it)
	{
		if ((*it)->getType() == type) {
			result.push_back(*it);
		}
	}

	return result;
}

std::vector<std::shared_ptr<Component> > EntityManagement::getAllComponentsByType(
	Component::Type type)
{
	return _component_management.getAllComponentsByType(type);
}

