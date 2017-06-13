#ifndef VULKAN_H
#define VULKAN_H

#define NOMINMAX
#define GLFW_INCLUDE_VULKAN
#include <glfw3.h>

#include <iostream>
#include <fstream>
#include <set>
#include <array>
#include <algorithm>
#include <memory>
#include <vector>
#include <string.h>

#include "glm/glm.hpp"
//#include <audio.h>


struct Vertex {
	glm::vec2 pos;
	glm::vec3 color;

	static VkVertexInputBindingDescription getBindingDescription() {
		VkVertexInputBindingDescription bindingDescription = {};
		bindingDescription.binding = 0;
		bindingDescription.stride = sizeof(Vertex);
		bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		return bindingDescription;
	}

	static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions() {
		std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions = {};

		attributeDescriptions[0].binding = 0;
		attributeDescriptions[0].location = 0;
		attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
		attributeDescriptions[0].offset = offsetof(Vertex, pos);

		attributeDescriptions[1].binding = 0;
		attributeDescriptions[1].location = 1;
		attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[1].offset = offsetof(Vertex, color);

		return attributeDescriptions;
	}
};

class Vulkan
{
public:
	Vulkan(GLFWwindow * glfw_win, int win_width, int win_height, bool debugMode);
	~Vulkan();

	void drawFrame();
	bool cleanUp();

	struct QueueFamilyIndices {
		int graphicsFamily = -1;
		int presentFamily = -1;

		bool isComplete() {
			return graphicsFamily >= 0 && presentFamily >= 0;
		}
	};

	struct SwapChainSupportDetails {
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
	};

private:
	bool _pre_init();

	//VULKAN INIT
	bool _init_vulkan();
	//VULKAN INSTANCE
	bool _create_instance();
	//VULKAN VALIDATION
	bool _check_validation_layer_support();
	std::vector<const char*> _get_required_extensions();
	static VKAPI_ATTR VkBool32 VKAPI_CALL _debug_callback(
		VkDebugReportFlagsEXT flags,
		VkDebugReportObjectTypeEXT objType,
		uint64_t obj,
		size_t location,
		int32_t code,
		const char* layerPrefix,
		const char* msg,
		void* userData);
	void _setup_debug_callback();
	static VkResult _create_debug_report_callback_EXT(
		VkInstance instance,
		const VkDebugReportCallbackCreateInfoEXT* pCreateInfo,
		const VkAllocationCallbacks* pAllocator,
		VkDebugReportCallbackEXT* pCallback);
	static void _destroy_debug_report_callback_EXT(
		VkInstance instance,
		VkDebugReportCallbackEXT callback,
		const VkAllocationCallbacks* pAllocator);
	//VULKAN PHYSICAL DEVICE
	bool _pick_physical_device();
	bool _is_device_suitable(VkPhysicalDevice device);
	QueueFamilyIndices _find_queue_families(VkPhysicalDevice device);
	//VULKAN LOGICAL DEVICE
	bool _create_logical_device();
	//VULKAN SURFACE
	bool _create_surface();
	//VULKAN DEVICE EXTENSION SUPPORT (SWAPCHAIN, ...)
	bool _check_device_extension_support(VkPhysicalDevice device);
	SwapChainSupportDetails _query_swap_chain_support(VkPhysicalDevice device);
	//VULKAN SWAPCHAIN CONFIG
	VkSurfaceFormatKHR _choose_swap_surface_format(
		const std::vector<VkSurfaceFormatKHR>& availableFormats);
	VkPresentModeKHR _choose_swap_present_mode(
		const std::vector<VkPresentModeKHR> availablePresentModes);
	VkExtent2D _choose_swap_extent(const VkSurfaceCapabilitiesKHR& capabilities);
	bool _create_swap_chain();
	bool _clean_up_swap_chain();
	//VULKAN IMAGE VIEWS
	bool _create_image_views();
	//VULKAN GFX PIPELINES
	bool _create_graphics_pipeline();
	static std::vector<char> _read_file(const std::string& filename);
	bool _create_shader_module(const std::vector<char>& code, VkShaderModule& shaderModule);
	//VULKAN RENDER PASS
	bool _create_render_pass();
	//VULKAN FRAMEBFFERS
	bool _create_framebuffers();
	//VULKAN COMMAND POOL
	bool _create_command_pool();
	//VULKAN BUFFER
	bool _create_buffer(
		VkDeviceSize size, VkBufferUsageFlags usage,
		VkMemoryPropertyFlags properties, VkBuffer& buffer,
		VkDeviceMemory& bufferMemory);
	bool _copy_buffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
	//VULKAN VERTEX BUFFER
	bool _create_vertex_buffer();
	uint32_t _find_memory_type(uint32_t typeFilter, VkMemoryPropertyFlags properties);
	//VULKAN COMMAND BUFFERS
	bool _create_command_buffers();
	//VULKAN SEMAPHORES
	bool _create_semaphores();
	bool _post_init();


	//if in debug
	bool _debug;

	GLFWwindow * _glfw_window;
	int _window_height;
	int _window_width;

	VkInstance _vulkan_instance;
	VkDebugReportCallbackEXT _debug_callback_instance;

	VkSurfaceKHR _vulkan_surface;

	VkPhysicalDevice _physical_device = VK_NULL_HANDLE;
	VkDevice _vulkan_device; //logical device

	VkQueue _graphics_queue;
	VkQueue _present_queue;

	VkSwapchainKHR _swap_chain;

	std::vector<VkImage> _swap_chain_images;

	VkFormat _swap_chain_image_format;
	VkExtent2D _swap_chain_extent;

	std::vector<VkImageView> _swap_chain_image_views;

	VkPipelineLayout _pipeline_layout;

	VkRenderPass _render_pass;

	VkPipeline _graphics_pipeline;

	std::vector<VkFramebuffer> _swap_chain_framebuffers;

	VkCommandPool _command_pool;

	VkBuffer _vertex_buffer;
	VkDeviceMemory _vertex_buffer_memory;

	std::vector<VkCommandBuffer> _command_buffers;

	//RENDERING
	VkSemaphore _image_available_semaphore;
	VkSemaphore _render_finished_semaphore;

	const std::vector<Vertex> _vertices = {
		{ { 0.0f, -0.5f },{ 1.0f, 0.0f, 1.0f } },
		{ { 0.5f, 0.5f },{ 1.0f, 1.0f, 0.0f } },
		{ { -0.5f, 0.5f },{ 0.0f, 1.0f, 1.0f } }
	};
};

#endif // VULKAN_H
