#include "igeometry.h"
#include <iostream>

IGeometry::IGeometry(const std::string& name) :
	IComponent(Type::Geometry)
	, mName(name)
{

}
