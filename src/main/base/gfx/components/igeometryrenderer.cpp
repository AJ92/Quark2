#include "igeometryrenderer.h"
#include <iostream>

IGeometryRenderer::IGeometryRenderer(std::string& name) :
	IComponent(Type::GeometryRenderer)
	, mName(name)
{

}
