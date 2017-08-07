#include "entity.h"
#include <iostream>


Entity::Entity(std::string name) :
	_identifier(name)
{

}

Entity::Entity() {

}

Entity::~Entity() {

}

std::string Entity::getIdentifier() {
	return _identifier;
}


