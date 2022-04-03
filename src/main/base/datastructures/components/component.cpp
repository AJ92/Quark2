#include "component.h"


Component::Component(Type type):
	mType(type)
{
	
}

Component::~Component() {

}

Component::Type Component::getType() {
	return mType;
}

void Component::init() {

}
void Component::update() {

}


