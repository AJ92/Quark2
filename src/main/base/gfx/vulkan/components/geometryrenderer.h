#ifndef VULKAN_GEOMETRY_RENDERER_H
#define VULKAN_GEOMETRY_RENDERER_H


#include "base/gfx/components/igeometryrenderer.h"
#include "base/gfx/vulkan/material.h"
#include "base/gfx/vulkan/geometry.h"

#include <vulkan/vulkan.h>
#include <glm/glm.hpp>


namespace vk_backend {

    class GeometryRenderer: public IGeometryRenderer
    {
    public:
        explicit GeometryRenderer(std::string& name);
        GeometryRenderer(
            const std::string & name, 
            const IMaterial & material,
            const IGeometry & geometry
        );
        virtual ~GeometryRenderer() = default;

        virtual void init();
        virtual void update();

    private:
        const IMaterial & mMaterial;
        const IGeometry & mGeometry;
    };

}

#endif // VULKAN_GEOMETRY_RENDERER_H
