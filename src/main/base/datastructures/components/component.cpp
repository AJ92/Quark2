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

void Component::start() {

}
void Component::update() {

}
void Component::end() {

}

