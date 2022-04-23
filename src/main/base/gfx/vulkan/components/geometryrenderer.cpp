#include "geometryrenderer.h"

namespace vk_backend {
    GeometryRenderer::GeometryRenderer(std::string& name) :
        IGeometryRenderer(name)
    {

    }

    GeometryRenderer::GeometryRenderer(
        std::string& name, 
        const IMaterial & material,
        const IGeometry & geometry
    ) :
        IGeometryRenderer(name),
        mMaterial(material),
        mGeometry(geometry)
    {
        
    }

    void GeometryRenderer::init(){

    }

    void GeometryRenderer::update(){

    }
}
