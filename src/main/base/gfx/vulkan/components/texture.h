#ifndef VULKAN_TEXTURE_H
#define VULKAN_TEXTURE_H

#include "base/gfx/components/itexture.h"
#include "base/gfx/vulkan/vulkan.h"
#include <vector>

namespace vk_backend {
    class Texture: public ITexture
    {
    public:
        explicit Texture(std::string& name);
        virtual ~Texture() = default;

        virtual void init();
        virtual void update();
    };
}

#endif // VULKAN_TEXTURE_H
