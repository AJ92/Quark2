#include "imaterial.h"
#include <iostream>

IMaterial::IMaterial(std::string& name) :
	IComponent(Type::Material)
	, mName(name)
{

}
