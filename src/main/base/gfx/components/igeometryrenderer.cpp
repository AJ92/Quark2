#include "igeometryrenderer.h"
#include <iostream>

IGeometryRenderer::IGeometryRenderer(const std::string & name) :
	IComponent(Type::GeometryRenderer), 
	mName(name)
{

}

IGeometryRenderer::IGeometryRenderer(
	const std::string & name, 
	const std::shared_ptr<IMaterial> material,
    const std::shared_ptr<IGeometry> geometry
) :
	IComponent(Type::GeometryRenderer), 
	mName(name),
	mMaterial(material),
	mGeometry(geometry)
{

}

void IGeometryRenderer::setMaterial(
	const std::shared_ptr<IMaterial> material
){
	mMaterial = material;
}

void IGeometryRenderer::setGeometry(
	const std::shared_ptr<IGeometry> geometry
){
	mGeometry = geometry;
}

const std::shared_ptr<IMaterial> IGeometryRenderer::getMaterial() const {
	return mMaterial;
}

const std::shared_ptr<IGeometry> IGeometryRenderer::getGeometry() const {
	return mGeometry;
}