#ifndef ENTITYMANAGEMENT_H
#define ENTITYMANAGEMENT_H

#include <memory>
#include <map>
#include <vector>

#include "base/datastructures/entity.h"
#include "base/datastructures/componentmanagement.h"

class EntityManagement
{
public:
	EntityManagement();
	~EntityManagement();

	std::shared_ptr<Entity> createEntity(std::string identifier);

	std::vector<std::shared_ptr<Component> > getAllComponentsFromEntity(
		std::shared_ptr<Entity> entity);

	std::vector<std::shared_ptr<Component> > getAllComponentsFromEntityByType(
		std::shared_ptr<Entity> entity, Component::Type type);

	std::vector<std::shared_ptr<Component> > getAllComponentsByType(Component::Type type);

	bool addComponentToEntity(
        std::shared_ptr<Entity> entity, std::shared_ptr<Component> comp);

	bool removeComponentFromEntity(
		std::shared_ptr<Entity> entity, std::shared_ptr<Component> comp);

    bool removeComponentsFromEntityByType(
		std::shared_ptr<Entity> entity, Component::Type type);

private:

	std::map<std::string, std::shared_ptr<Entity> > _identifier_entity_map;
	std::map<std::string, std::vector<std::shared_ptr<Component> > > _identifier_component_list_map;

	ComponentManagement _component_management;
};

#endif // ENTITYMANAGEMENT_H
