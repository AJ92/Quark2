#ifndef VULKAN_GEOMETRY_RENDERER_H
#define VULKAN_GEOMETRY_RENDERER_H


#include "base/gfx/components/igeometryrenderer.h"
#include "base/gfx/components/imaterial.h"
#include "base/gfx/components/igeometry.h"

#include <vulkan/vulkan.h>
#include <glm/glm.hpp>


namespace vk_backend {

    class GeometryRenderer: public IGeometryRenderer
    {
    public:
        explicit GeometryRenderer(std::string& name);
        GeometryRenderer(
            const std::string & name, 
            const std::shared_ptr<IMaterial> material,
            const std::shared_ptr<IGeometry> geometry
        );
        virtual ~GeometryRenderer() = default;

        virtual void init();
        virtual void update();

    private:

    };

}

#endif // VULKAN_GEOMETRY_RENDERER_H
