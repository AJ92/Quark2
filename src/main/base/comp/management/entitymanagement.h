#ifndef ENTITYMANAGEMENT_H
#define ENTITYMANAGEMENT_H

#include <memory>
#include <map>
#include <vector>

#include "base/comp/management/entity.h"
#include "base/comp/management/componentmanagement.h"

class EntityManagement
{
public:
	EntityManagement();
	~EntityManagement();

	std::shared_ptr<Entity> createEntity(std::string identifier);

	std::vector<std::shared_ptr<IComponent> > getAllComponentsFromEntity(
		std::shared_ptr<Entity> entity);

	std::vector<std::shared_ptr<IComponent> > getAllComponentsFromEntityByType(
		std::shared_ptr<Entity> entity, IComponent::Type type);

	std::vector<std::shared_ptr<IComponent> > getAllComponentsByType(IComponent::Type type);

	bool addComponentToEntity(
        std::shared_ptr<Entity> entity, std::shared_ptr<IComponent> comp);

	bool removeComponentFromEntity(
		std::shared_ptr<Entity> entity, std::shared_ptr<IComponent> comp);

    bool removeComponentsFromEntityByType(
		std::shared_ptr<Entity> entity, IComponent::Type type);

private:

	std::map<std::string, std::shared_ptr<Entity> > mIdentifierEntityMap;
	std::map<std::string, std::vector<std::shared_ptr<IComponent> > > mIdentifierComponentListMap;

	ComponentManagement mComponentManagement;
};

#endif // ENTITYMANAGEMENT_H
