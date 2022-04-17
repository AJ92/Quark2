#include "igeometry.h"
#include <iostream>

IGeometry::IGeometry() :
	IComponent(Type::Geometry)
{

}

std::vector<Vertex> & IGeometry::getVertexVector(){
	return mVertices;
}
std::vector<uint32_t> & IGeometry::getIndexVector(){
	return mIndices;
}
