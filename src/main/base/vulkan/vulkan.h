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

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <chrono>

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

struct UniformBufferObject {
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 proj;
};

class Vulkan
{
public:
	Vulkan(GLFWwindow * glfw_win, int win_width, int win_height, bool debugMode);
	~Vulkan();

	void updateUniformBuffer();
	void drawFrame();
	void windowResize(int width, int height);

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
	bool _recreate_swap_chain();
	bool _clean_up_swap_chain();
//VULKAN IMAGE VIEWS
	VkImageView _create_image_view(
		VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);
	bool _create_image_views();
//VULKAN DESCRIPTORSETS
	bool _create_descriptor_set_layout();
//VULKAN GFX PIPELINES
	bool _create_graphics_pipeline();
	static std::vector<char> _read_file(const std::string& filename);
	bool _create_shader_module(const std::vector<char>& code, VkShaderModule& shaderModule);
//VULKAN RENDER PASS
	bool _create_render_pass();
//VULKAN FRAMEBFFERS
	bool _create_frame_buffers();
//VULKAN COMMAND POOL
	bool _create_command_pool();
//VULKAN SINGLE TIME COMMANDS
	VkCommandBuffer _begin_single_time_commands();
	void _end_single_time_commands(VkCommandBuffer commandBuffer);
//VULKAN DEPTH RES
	VkFormat _find_supported_format(
		const std::vector<VkFormat>& candidates,
		VkImageTiling tiling,
		VkFormatFeatureFlags features);
	VkFormat _find_depth_format();
	bool _has_stencil_component(VkFormat format);
	bool _create_depth_resources();
//VULKAN TEXTURE IMAGE
	bool _create_image(
		uint32_t width, uint32_t height, VkFormat format,
		VkImageTiling tiling, VkImageUsageFlags usage,
		VkMemoryPropertyFlags properties, VkImage& image,
		VkDeviceMemory& imageMemory);
	bool _create_texture_image();
	bool _transition_image_layout(
		VkImage image, VkFormat format,
		VkImageLayout oldLayout, VkImageLayout newLayout);
	bool _copy_buffer_to_image(
		VkBuffer buffer, VkImage image,
		uint32_t width, uint32_t height);
	bool _create_texture_image_view();
	bool _create_texture_sampler();
//VULKAN DESCRIPTOR POOL
	bool _create_descriptor_pool();
	bool _create_descriptor_set();
//VULKAN BUFFER
	bool _create_buffer(
		VkDeviceSize size, VkBufferUsageFlags usage,
		VkMemoryPropertyFlags properties, VkBuffer& buffer,
		VkDeviceMemory& bufferMemory);
	bool _copy_buffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
//VULKAN BUFFER
	bool _create_vertex_buffer();
	bool _create_index_buffer();
	bool _create_uniform_buffer();
	uint32_t _find_memory_type(uint32_t typeFilter, VkMemoryPropertyFlags properties);
//VULKAN COMMAND BUFFERS
	bool _create_command_buffers();
//VULKAN SEMAPHORES
	bool _create_semaphores();
	bool _post_init();


	//if in debug
	bool						_debug;

	GLFWwindow *				_glfw_window;
	int							_window_height;
	int							_window_width;

	VkInstance					_vulkan_instance;
	VkDebugReportCallbackEXT	_debug_callback_instance;

	VkSurfaceKHR				_vulkan_surface;

	VkPhysicalDevice			_physical_device = VK_NULL_HANDLE;
	VkDevice					_vulkan_device; //logical device

	VkQueue						_graphics_queue;
	VkQueue						_present_queue;

	VkSwapchainKHR				_swap_chain;

	std::vector<VkImage>		_swap_chain_images;

	VkFormat					_swap_chain_image_format;
	VkExtent2D					_swap_chain_extent;

	std::vector<VkImageView>	_swap_chain_image_views;

	VkImage						_depth_image;
	VkDeviceMemory				_depth_image_memory;
	VkImageView					_depth_image_view;

	VkDescriptorSetLayout		_descriptor_set_layout;
	VkPipelineLayout			_pipeline_layout;

	VkRenderPass				_render_pass;

	VkPipeline					_graphics_pipeline;

	std::vector<VkFramebuffer>	_swap_chain_framebuffers;

	VkCommandPool				_command_pool;

	VkImage						_texture_image;
	VkDeviceMemory				_texture_image_memory;
	VkImageView					_texture_image_view;
	VkSampler					_texture_sampler;

	VkDescriptorPool			_descriptor_pool;
	VkDescriptorSet				_descriptor_set;

	VkBuffer					_vertex_buffer;
	VkDeviceMemory				_vertex_buffer_memory;
	VkBuffer					_index_buffer;
	VkDeviceMemory				_index_buffer_memory;
	VkBuffer					_uniform_buffer;
	VkDeviceMemory				_uniform_buffer_memory;

	std::vector<VkCommandBuffer> _command_buffers;

	//RENDERING
	VkSemaphore _image_available_semaphore;
	VkSemaphore _render_finished_semaphore;

	const std::vector<Vertex> _vertices = {
		{ { -0.5f, -0.5f, 0.0f }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f } },
		{ {  0.5f, -0.5f, 0.0f }, { 0.0f, 1.0f, 0.0f }, { 1.0f, 0.0f } },
		{ {  0.5f,  0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f } },
		{ { -0.5f,  0.5f, 0.0f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f } },

		{ { -0.5f, -0.5f, -0.5f }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f } },
		{ {  0.5f, -0.5f, -0.5f }, { 0.0f, 1.0f, 0.0f }, { 1.0f, 0.0f } },
		{ {  0.5f,  0.5f, -0.5f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f } },
		{ { -0.5f,  0.5f, -0.5f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f } }
	};
	
	const std::vector<uint32_t> _indices = {
		0, 1, 2, 2, 3, 0,
		4, 5, 6, 6, 7, 4
	};

	//Test model loading...
	const std::string MODEL_PATH = "models/chalet.obj";
	const std::string TEXTURE_PATH = "textures/chalet.jpg";
};

#endif // VULKAN_H
