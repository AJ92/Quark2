#include "entitymanagement.h"
#include <iostream>
#include <cstdlib>
#include <ctime>



EntityManagement::EntityManagement() {

}

EntityManagement::~EntityManagement() {

}

std::shared_ptr<Entity> EntityManagement::createEntity(std::string identifier){
    //check if identifier is not used yet...
    std::map<std::string, std::shared_ptr<Entity> >::iterator it_map;
	it_map = mIdentifierEntityMap.find(identifier);

	if (it_map == mIdentifierEntityMap.end()) {
        //identifier not in the map yet so entity can use it
        std::shared_ptr<Entity> entity = std::make_shared<Entity>(identifier);
        mIdentifierEntityMap[identifier] = entity;
        return entity;
    }

    //try a few times to create a new identifier...
    std::srand(std::time(0));
	for(int i = 0; i < 3; i++){
        std::string new_identifier = identifier + "_" + std::to_string(std::rand());
        it_map = mIdentifierEntityMap.find(identifier);
        if (it_map == mIdentifierEntityMap.end()) {
            std::shared_ptr<Entity> entity = std::make_shared<Entity>(new_identifier);
            mIdentifierEntityMap[new_identifier] = entity;
            return entity;
        }
	}
    return std::shared_ptr<Entity>();
}

std::vector<std::shared_ptr<Component> > EntityManagement::getAllComponentsFromEntity(
	std::shared_ptr<Entity> entity)
{
	std::map<std::string, std::vector<std::shared_ptr<Component> > >::iterator it_map;
	it_map = mIdentifierComponentListMap.find(entity->getIdentifier());

	if (it_map == mIdentifierComponentListMap.end()) {
		return std::vector<std::shared_ptr<Component> >();
	}

	return it_map->second;
}

std::vector<std::shared_ptr<Component> > EntityManagement::getAllComponentsFromEntityByType(
	std::shared_ptr<Entity> entity, Component::Type type)
{
	std::vector<std::shared_ptr<Component> > result;

	std::map<std::string, std::vector<std::shared_ptr<Component> > >::iterator it_map;
	it_map = mIdentifierComponentListMap.find(entity->getIdentifier());

	if (it_map == mIdentifierComponentListMap.end()) {
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
	return mComponentManagement.getAllComponentsByType(type);
}

bool EntityManagement::addComponentToEntity(
    std::shared_ptr<Entity> entity, std::shared_ptr<Component> comp)
{
	std::map<std::string, std::vector<std::shared_ptr<Component> > >::iterator it_map;
	it_map = mIdentifierComponentListMap.find(entity->getIdentifier());

	if (it_map == mIdentifierComponentListMap.end()) {
        return false;
	}

    it_map->second.push_back(comp);
    return true;
}

bool EntityManagement::removeComponentFromEntity(
    std::shared_ptr<Entity> entity, std::shared_ptr<Component> comp)
{
    std::map<std::string, std::vector<std::shared_ptr<Component> > >::iterator it_map;
	it_map = mIdentifierComponentListMap.find(entity->getIdentifier());

	if (it_map == mIdentifierComponentListMap.end()) {
        return false;
	}


	std::vector<std::shared_ptr<Component> >::iterator it_vec;
	it_vec = std::find(it_map->second.begin(), it_map->second.end(), comp);

	if (it_vec != it_map->second.end()) {
		//delete the found element from the vector
		it_map->second.erase(it_vec);
		return true;
	}

    return false;
}

bool EntityManagement::removeComponentsFromEntityByType(
    std::shared_ptr<Entity> entity, Component::Type type)
{
    std::map<std::string, std::vector<std::shared_ptr<Component> > >::iterator it_map;
	it_map = mIdentifierComponentListMap.find(entity->getIdentifier());

	if (it_map == mIdentifierComponentListMap.end()) {
        return false;
	}


	for (std::vector<std::shared_ptr<Component> >::iterator it = it_map->second.begin();
		it != it_map->second.end();
		++it)
	{
		if ((*it)->getType() == type) {
			it_map->second.erase(it);
		}
	}

    return false;
}

