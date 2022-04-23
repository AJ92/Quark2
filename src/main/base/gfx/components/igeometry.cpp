#include "igeometry.h"
#include <iostream>

IGeometry::IGeometry(std::string& name) :
	IComponent(Type::Geometry)
	, mName(name)
{

}
