#include "geometryrenderer.h"

namespace vk_backend {
    GeometryRenderer::GeometryRenderer(std::string& name) :
        IGeometryRenderer(name)
    {

    }

    GeometryRenderer::GeometryRenderer(
        const std::string & name, 
        const std::shared_ptr<IMaterial> material,
        const std::shared_ptr<IGeometry> geometry
    ) :
        IGeometryRenderer(name, material, geometry)
    {
        
    }

    void GeometryRenderer::init(){

    }

    void GeometryRenderer::update(){

    }
}
