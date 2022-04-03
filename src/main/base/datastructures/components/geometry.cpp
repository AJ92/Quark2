#include "geometry.h"
#include <iostream>

Geometry::Geometry() :
	Component(Type::Geometry)
{

}

Geometry::~Geometry()
{

}

void Geometry::update() {
	
}

void Geometry::init() {
	
}

std::vector<Vertex> & Geometry::getVertexVector(){
	return mVertices;
}
std::vector<uint32_t> & Geometry::getIndexVector(){
	return mIndices;
}
