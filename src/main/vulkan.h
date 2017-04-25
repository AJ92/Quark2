#ifndef VULKAN_H
#define VULKAN_H

#define NOMINMAX
#define GLFW_INCLUDE_VULKAN
#include <glfw3.h>

#include <iostream>
#include <set>
#include <algorithm>
#include <memory>
#include <audio.h>

#include "vdeleter.h"



class Vulkan
{
public:
	Vulkan(GLFWwindow * glfw_win, int win_width, int win_height, bool debugMode);
	~Vulkan();

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
	VkSurfaceFormatKHR _choose_swap_surface_format(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	VkPresentModeKHR _choose_swap_present_mode(const std::vector<VkPresentModeKHR> availablePresentModes);
	VkExtent2D _choose_swap_extent(const VkSurfaceCapabilitiesKHR& capabilities);
	bool _create_swap_chain();

	bool _post_init();


	//if in debug
	bool _debug;

	GLFWwindow * _glfw_window;
	int _window_height;
	int _window_width;

	VDeleter<VkInstance> _vulkan_instance{ vkDestroyInstance };
	VDeleter<VkDebugReportCallbackEXT> _debug_callback_instance{ _vulkan_instance, _destroy_debug_report_callback_EXT };

	VDeleter<VkSurfaceKHR> _vulkan_surface{ _vulkan_instance, vkDestroySurfaceKHR };

	//Automagically destroyed once _vulkan_instance (VkInstance) is destroyed
	VkPhysicalDevice _physical_device = VK_NULL_HANDLE;

	//must be under _vulkan_instance to get deleted before it!!!
	VDeleter<VkDevice> _vulkan_device{ vkDestroyDevice }; //logical device

	//Automagically destroyed once the device is cleaned up...
	VkQueue _graphics_queue;

	VkQueue _present_queue;

	VDeleter<VkSwapchainKHR> _swap_chain{ _vulkan_device, vkDestroySwapchainKHR };

	std::vector<VkImage> _swap_chain_images;

	VkFormat _swap_chain_image_format;
	VkExtent2D _swap_chain_extent;

};

#endif // VULKAN_H