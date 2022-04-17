#include "icomponent.h"


IComponent::IComponent(Type type):
	mType(type)
{
	
}

IComponent::Type IComponent::getType() {
	return mType;
}


