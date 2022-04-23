#ifndef VULKAN_GEOMETRY_H
#define VULKAN_GEOMETRY_H


#include "base/gfx/components/igeometry.h"

#include <vulkan/vulkan.h>
#include <glm/glm.hpp>

#include <array>
#include <vector>

namespace vk_backend {

    struct Vertex {
        glm::vec3 pos;
        glm::vec3 color;
        glm::vec2 texCoord;

        static VkVertexInputBindingDescription getBindingDescription() {
            VkVertexInputBindingDescription bindingDescription = {};
            bindingDescription.binding = 0;
            bindingDescription.stride = sizeof(Vertex);
            bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

            return bindingDescription;
        }

        static std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions() {
            std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions = {};

            attributeDescriptions[0].binding = 0;
            attributeDescriptions[0].location = 0;
            attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
            attributeDescriptions[0].offset = offsetof(Vertex, pos);

            attributeDescriptions[1].binding = 0;
            attributeDescriptions[1].location = 1;
            attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
            attributeDescriptions[1].offset = offsetof(Vertex, color);

            attributeDescriptions[2].binding = 0;
            attributeDescriptions[2].location = 2;
            attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
            attributeDescriptions[2].offset = offsetof(Vertex, texCoord);

            return attributeDescriptions;
        }
    };


    class Geometry: public IGeometry
    {
    public:
        explicit Geometry(std::string& name);
        Geometry(
            std::string& name, 
            const std::vector<Vertex> & vertices,
            const std::vector<uint32_t> & indices
        );
        virtual ~Geometry() = default;

        virtual void init();
        virtual void update();

        const std::vector<Vertex> & getVertexVector();
	    const std::vector<uint32_t> & getIndexVector();

    private:
        std::vector<Vertex> mVertexVector;
        std::vector<uint32_t> mIndexVector;
    };

}

#endif // VULKAN_GEOMETRY_H
