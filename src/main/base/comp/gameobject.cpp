#include "gameobject.h"
#include <iostream>

GameObject::GameObject() :
	IComponent(Type::GameObject)
{

}

GameObject::GameObject(
	const std::string & name, 
	std::shared_ptr<GameObject> parent
) : 
	IComponent(Type::GameObject),
	mParent(parent)
{
	
}

void GameObject::update() {
	
}

void GameObject::init() {
	
}

const std::string GameObject::getName() const{
	if(mEntity != nullptr){
		return mEntity->getIdentifier();
	}
	return "";
}

const std::shared_ptr<GameObject> GameObject::getParent() const{
	return mParent;
}
