#ifndef VULKAN_SHADER_H
#define VULKAN_SHADER_H

#include "base/gfx/components/ishader.h"
#include <vector>

namespace vk_backend {
    class Shader: public IShader
    {
    public:
        explicit Shader(std::string& name);
        virtual ~Shader() = default;

        virtual void init();
        virtual void update();
    };
}

#endif // VULKAN_SHADER_H
