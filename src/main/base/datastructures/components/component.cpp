#include "component.h"


Component::Component(Type type):
	_type(type)
{
	
}

Component::~Component() {

}

Component::Type Component::getType() {
	return _type;
}

void Component::init() {

}
void Component::update() {

}


