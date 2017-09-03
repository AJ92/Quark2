#include "vulkan.h"

#ifndef _STB_IMAGE_INCLUDE_GUARD
#define _STB_IMAGE_INCLUDE_GUARD
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#endif

const std::vector<const char*> validationLayers = {
	"VK_LAYER_LUNARG_standard_validation"
};

const std::vector<const char*> deviceExtensions = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

Vulkan::Vulkan(GLFWwindow * glfw_win, int win_width, int win_height, bool debugMode) :
	_debug(debugMode),
	_glfw_window(glfw_win),
	_window_width(win_width),
	_window_height(win_height)
{
	_pre_init();
	_init_vulkan();
	_post_init();
}

Vulkan::~Vulkan() {

}

///////////////////////////////////////////////
//
//		PRIVATE

bool Vulkan::_pre_init() {
	return true;
}

bool Vulkan::_init_vulkan() {
	_create_instance();
	_setup_debug_callback();
	_create_surface();
	_pick_physical_device();
	_create_logical_device();
	_create_swap_chain();
	_create_image_views();
	_create_render_pass();
	_create_descriptor_set_layout();
	_create_graphics_pipeline();
	_create_command_pool();
	_create_depth_resources();
	_create_frame_buffers();
	_create_texture_image();
	_create_texture_image_view();
	_create_texture_sampler();
	_create_vertex_buffer();
	_create_index_buffer();
	_create_uniform_buffer();
	_create_descriptor_pool();
	_create_descriptor_set();
	_create_command_buffers();
	_create_semaphores();
	return true;
}

bool Vulkan::_create_instance() {
	if (_debug && !_check_validation_layer_support()) {
		throw std::runtime_error("validation layers requested, but not available!");
	}


	uint32_t extensionCount = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
	std::vector<VkExtensionProperties> extensions(extensionCount);
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

	std::cout << "available extensions:" << std::endl;
	for (const auto& extension : extensions) {
		std::cout << "\t" << extension.extensionName << std::endl;
	}

	VkApplicationInfo appInfo = {};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "quark engine";
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "quark engine";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_0;

	VkInstanceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;

	auto req_extensions = _get_required_extensions();

	std::cout << "required extensions:" << std::endl;
	for (const auto& extension : req_extensions) {
		std::cout << "\t" << extension << std::endl;
	}

	createInfo.enabledExtensionCount = req_extensions.size();
	createInfo.ppEnabledExtensionNames = req_extensions.data();


	if (_debug) {
		createInfo.enabledLayerCount = validationLayers.size();
		createInfo.ppEnabledLayerNames = validationLayers.data();
	}
	else {
		createInfo.enabledLayerCount = 0;
	}

	if (vkCreateInstance(&createInfo, nullptr, &_vulkan_instance) != VK_SUCCESS) {
		throw std::runtime_error("failed to create instance!");
		return false;
	}
	return true;
}

bool Vulkan::_check_validation_layer_support() {
	uint32_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

	std::vector<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

	for (const char* layerName : validationLayers) {
		bool layerFound = false;

		for (const auto& layerProperties : availableLayers) {
			if (strcmp(layerName, layerProperties.layerName) == 0) {
				layerFound = true;
				break;
			}
		}

		if (!layerFound) {
			return false;
		}
	}

	return true;
}

std::vector<const char*> Vulkan::_get_required_extensions() {
	std::vector<const char*> extensions;

	unsigned int glfwExtensionCount = 0;
	const char** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	for (unsigned int i = 0; i < glfwExtensionCount; i++) {
		extensions.push_back(glfwExtensions[i]);
	}

	if (_debug) {
		extensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
	}

	return extensions;
}

VKAPI_ATTR VkBool32 VKAPI_CALL Vulkan::_debug_callback(
	VkDebugReportFlagsEXT flags,
	VkDebugReportObjectTypeEXT objType,
	uint64_t obj,
	size_t location,
	int32_t code,
	const char* layerPrefix,
	const char* msg,
	void* userData) {

	std::cerr << "validation layer: " << msg << std::endl;

	return VK_FALSE;
}

void Vulkan::_setup_debug_callback() {
	if (!_debug) return;

	VkDebugReportCallbackCreateInfoEXT createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
	createInfo.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;
	createInfo.pfnCallback = Vulkan::_debug_callback;

	if (_create_debug_report_callback_EXT(_vulkan_instance, &createInfo, nullptr, &_debug_callback_instance) != VK_SUCCESS) {
		throw std::runtime_error("failed to set up debug callback!");
	}
}

VkResult Vulkan::_create_debug_report_callback_EXT(VkInstance instance, const VkDebugReportCallbackCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugReportCallbackEXT* pCallback) {
	auto func = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugReportCallbackEXT");
	if (func != nullptr) {
		return func(instance, pCreateInfo, pAllocator, pCallback);
	}
	else {
		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}
}

void Vulkan::_destroy_debug_report_callback_EXT(VkInstance instance, VkDebugReportCallbackEXT callback, const VkAllocationCallbacks* pAllocator) {
	auto func = (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugReportCallbackEXT");
	if (func != nullptr) {
		func(instance, callback, pAllocator);
	}
}

bool Vulkan::_pick_physical_device() {
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(_vulkan_instance, &deviceCount, nullptr);
	if (deviceCount == 0) {
		throw std::runtime_error("failed to find GPUs with Vulkan support!");
		return false;
	}

	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(_vulkan_instance, &deviceCount, devices.data());

	for (const auto& device : devices) {
		if (_is_device_suitable(device)) {
			_physical_device = device;
			break;
		}
	}

	if (_physical_device == VK_NULL_HANDLE) {
		throw std::runtime_error("failed to find a suitable GPU!");
		return false;
	}
	return true;
}

bool Vulkan::_is_device_suitable(VkPhysicalDevice device) {
	//find queue families
	QueueFamilyIndices indices = _find_queue_families(device);

	//check if extensions are supported
	bool extensionsSupported = _check_device_extension_support(device);

	//check if swapchain extension is adequate
	bool swapChainAdequate = false;
	if (extensionsSupported) {
		SwapChainSupportDetails swapChainSupport = _query_swap_chain_support(device);
		swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
	}

	VkPhysicalDeviceFeatures supportedFeatures;
	vkGetPhysicalDeviceFeatures(device, &supportedFeatures);

	//check if device has what it takes to be a great vulkan device!
	return indices.isComplete() &&
		extensionsSupported &&
		swapChainAdequate &&
		supportedFeatures.samplerAnisotropy;
}

Vulkan::QueueFamilyIndices Vulkan::_find_queue_families(VkPhysicalDevice device) {
	QueueFamilyIndices indices;

	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

	int i = 0;
	for (const auto& queueFamily : queueFamilies) {
		if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
			indices.graphicsFamily = i;
		}

		VkBool32 presentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(device, i, _vulkan_surface, &presentSupport);

		if (queueFamily.queueCount > 0 && presentSupport) {
			indices.presentFamily = i;
		}

		if (indices.isComplete()) {
			break;
		}

		i++;
	}

	return indices;
}

bool Vulkan::_create_logical_device() {
	QueueFamilyIndices indices = _find_queue_families(_physical_device);

	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
	std::set<int> uniqueQueueFamilies = { indices.graphicsFamily, indices.presentFamily };

	float queuePriority = 1.0f;
	for (int queueFamily : uniqueQueueFamilies) {
		VkDeviceQueueCreateInfo queueCreateInfo = {};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = queueFamily;
		queueCreateInfo.queueCount = 1;
		queueCreateInfo.pQueuePriorities = &queuePriority;
		queueCreateInfos.push_back(queueCreateInfo);
	}

	VkPhysicalDeviceFeatures deviceFeatures = {};
	deviceFeatures.samplerAnisotropy = VK_TRUE;

	VkDeviceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

	createInfo.pQueueCreateInfos = queueCreateInfos.data();
	createInfo.queueCreateInfoCount = (uint32_t)queueCreateInfos.size();

	createInfo.pEnabledFeatures = &deviceFeatures;

	createInfo.enabledExtensionCount = deviceExtensions.size();
	createInfo.ppEnabledExtensionNames = deviceExtensions.data();

	if (_debug) {
		createInfo.enabledLayerCount = validationLayers.size();
		createInfo.ppEnabledLayerNames = validationLayers.data();
	}
	else {
		createInfo.enabledLayerCount = 0;
	}

	if (vkCreateDevice(_physical_device, &createInfo, nullptr, &_vulkan_device) != VK_SUCCESS) {
		throw std::runtime_error("failed to create logical device!");
	}

	vkGetDeviceQueue(_vulkan_device, indices.graphicsFamily, 0, &_graphics_queue);
	vkGetDeviceQueue(_vulkan_device, indices.presentFamily, 0, &_present_queue);

	return true;
}

bool Vulkan::_create_surface() {
	if (glfwCreateWindowSurface(_vulkan_instance, _glfw_window, nullptr, &_vulkan_surface) != VK_SUCCESS) {
		throw std::runtime_error("failed to create window surface!");
		return false;
	}
	return true;
}

bool Vulkan::_check_device_extension_support(VkPhysicalDevice device) {
	uint32_t extensionCount;
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

	std::vector<VkExtensionProperties> availableExtensions(extensionCount);
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

	std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

	std::cout << "device extensions:" << std::endl;
	for (const auto& extension : availableExtensions) {
		std::cout << "\t" << extension.extensionName << std::endl;
		requiredExtensions.erase(extension.extensionName);
	}

	return requiredExtensions.empty();
}

Vulkan::SwapChainSupportDetails Vulkan::_query_swap_chain_support(VkPhysicalDevice device) {
	SwapChainSupportDetails details;

	//capabilities
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, _vulkan_surface, &details.capabilities);

	//formats
	uint32_t formatCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, _vulkan_surface, &formatCount, nullptr);
	if (formatCount != 0) {
		details.formats.resize(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, _vulkan_surface, &formatCount, details.formats.data());
	}

	//present modes
	uint32_t presentModeCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(device, _vulkan_surface, &presentModeCount, nullptr);
	if (presentModeCount != 0) {
		details.presentModes.resize(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, _vulkan_surface, &presentModeCount, details.presentModes.data());
	}

	return details;
}

VkSurfaceFormatKHR Vulkan::_choose_swap_surface_format(const std::vector<VkSurfaceFormatKHR>& availableFormats) {
	//surface has no prefered format we use what we want...
	if (availableFormats.size() == 1 && availableFormats[0].format == VK_FORMAT_UNDEFINED) {
		std::cout << "any desired swap surface format available. using:" << std::endl;
		std::cout << "\tformat: VK_FORMAT_B8G8R8A8_UNORM " << VK_FORMAT_B8G8R8A8_UNORM << std::endl;
		std::cout << "\tcolorSpace: VK_COLOR_SPACE_SRGB_NONLINEAR_KHR " << VK_COLOR_SPACE_SRGB_NONLINEAR_KHR << std::endl;
		return { VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR };
	}

	//check if desired combo is available....
	for (const auto& availableFormat : availableFormats) {
		if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
			std::cout << "desired swap surface format available. using:" << std::endl;
			std::cout << "\tformat: VK_FORMAT_B8G8R8A8_UNORM " << availableFormat.format << std::endl;
			std::cout << "\tcolorSpace: VK_COLOR_SPACE_SRGB_NONLINEAR_KHR " << availableFormat.colorSpace << std::endl;
			return availableFormat;
		}
	}

	//desired values are not present in available formats...
	//use the first one

	std::cout << "desired swap surface format NOT available. using:" << std::endl;
	std::cout << "\tformat:" << availableFormats[0].format << std::endl;
	std::cout << "\tcolorSpace:" << availableFormats[0].colorSpace << std::endl;

	return availableFormats[0];
}

VkPresentModeKHR Vulkan::_choose_swap_present_mode(const std::vector<VkPresentModeKHR> availablePresentModes) {
	std::cout << "selecting swap present mode:" << std::endl;
	for (const auto& availablePresentMode : availablePresentModes) {
		if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
			std::cout << "\t" << "VK_PRESENT_MODE_MAILBOX_KHR" << std::endl;
			return availablePresentMode;
		}
		else if (availablePresentMode == VK_PRESENT_MODE_IMMEDIATE_KHR) {
			std::cout << "\t" << "VK_PRESENT_MODE_IMMEDIATE_KHR" << std::endl;
			return availablePresentMode;
		}
	}
	std::cout << "\t" << "VK_PRESENT_MODE_FIFO_KHR" << std::endl;
	return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D Vulkan::_choose_swap_extent(const VkSurfaceCapabilitiesKHR& capabilities) {
	if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
		return capabilities.currentExtent;
	}
	else {
		VkExtent2D actualExtent = { _window_width, _window_height };

		actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
		actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));

		return actualExtent;
	}
}

bool Vulkan::_create_swap_chain() {
	SwapChainSupportDetails swapChainSupport = _query_swap_chain_support(_physical_device);

	VkSurfaceFormatKHR surfaceFormat = _choose_swap_surface_format(swapChainSupport.formats);
	VkPresentModeKHR presentMode = _choose_swap_present_mode(swapChainSupport.presentModes);
	VkExtent2D extent = _choose_swap_extent(swapChainSupport.capabilities);

	uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
	if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
		imageCount = swapChainSupport.capabilities.maxImageCount;
	}

	VkSwapchainCreateInfoKHR createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createInfo.surface = _vulkan_surface;
	createInfo.minImageCount = imageCount;
	createInfo.imageFormat = surfaceFormat.format;
	createInfo.imageColorSpace = surfaceFormat.colorSpace;
	createInfo.imageExtent = extent;
	createInfo.imageArrayLayers = 1;
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	QueueFamilyIndices indices = _find_queue_families(_physical_device);
	uint32_t queueFamilyIndices[] = { (uint32_t)indices.graphicsFamily, (uint32_t)indices.presentFamily };

	std::cout << "selecting swap chain image sharing mode:" << std::endl;
	if (indices.graphicsFamily != indices.presentFamily) {
		std::cout << "\t" << "VK_SHARING_MODE_CONCURRENT" << std::endl;
		createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		createInfo.queueFamilyIndexCount = 2;
		createInfo.pQueueFamilyIndices = queueFamilyIndices;
	}
	else {
		std::cout << "\t" << "VK_SHARING_MODE_EXCLUSIVE" << std::endl;
		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		createInfo.queueFamilyIndexCount = 0; // Optional
		createInfo.pQueueFamilyIndices = nullptr; // Optional
	}

	createInfo.preTransform = swapChainSupport.capabilities.currentTransform; //do not pretransform
	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR; //do not blend with window

	createInfo.presentMode = presentMode;
	createInfo.clipped = VK_TRUE; //dont bother with pixels that are not visible...

	//TODO:
	createInfo.oldSwapchain = VK_NULL_HANDLE; //handle to old swapchain...

	if (vkCreateSwapchainKHR(_vulkan_device, &createInfo, nullptr, &_swap_chain) != VK_SUCCESS) {
		throw std::runtime_error("failed to create swap chain!");
		return false;
	}

	vkGetSwapchainImagesKHR(_vulkan_device, _swap_chain, &imageCount, nullptr);
	_swap_chain_images.resize(imageCount);
	vkGetSwapchainImagesKHR(_vulkan_device, _swap_chain, &imageCount, _swap_chain_images.data());


	_swap_chain_image_format = surfaceFormat.format;
	_swap_chain_extent = extent;

	return true;
}

bool Vulkan::_recreate_swap_chain() {
	vkDeviceWaitIdle(_vulkan_device);

	_clean_up_swap_chain();

	_create_swap_chain();
	_create_image_views();
	_create_render_pass();
	_create_graphics_pipeline();
	_create_depth_resources();
	_create_frame_buffers();
	_create_command_buffers();
	return true;
}

bool Vulkan::_clean_up_swap_chain() {
	vkDestroyImageView(_vulkan_device, _depth_image_view, nullptr);
	vkDestroyImage(_vulkan_device, _depth_image, nullptr);
	vkFreeMemory(_vulkan_device, _depth_image_memory, nullptr);

	for (size_t i = 0; i < _swap_chain_framebuffers.size(); i++) {
		vkDestroyFramebuffer(_vulkan_device, _swap_chain_framebuffers[i], nullptr);
	}

	vkFreeCommandBuffers(_vulkan_device, _command_pool, static_cast<uint32_t>(_command_buffers.size()), _command_buffers.data());

	vkDestroyPipeline(_vulkan_device, _graphics_pipeline, nullptr);
	vkDestroyPipelineLayout(_vulkan_device, _pipeline_layout, nullptr);
	vkDestroyRenderPass(_vulkan_device, _render_pass, nullptr);

	for (size_t i = 0; i < _swap_chain_image_views.size(); i++) {
		vkDestroyImageView(_vulkan_device, _swap_chain_image_views[i], nullptr);
	}

	vkDestroySwapchainKHR(_vulkan_device, _swap_chain, nullptr);
	return true;
}

VkImageView Vulkan::_create_image_view(
	VkImage image, VkFormat format, VkImageAspectFlags aspectFlags)
{
	VkImageViewCreateInfo viewInfo = {};
	viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	viewInfo.image = image;
	viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	viewInfo.format = format;
	viewInfo.subresourceRange.aspectMask = aspectFlags;
	viewInfo.subresourceRange.baseMipLevel = 0;
	viewInfo.subresourceRange.levelCount = 1;
	viewInfo.subresourceRange.baseArrayLayer = 0;
	viewInfo.subresourceRange.layerCount = 1;

	VkImageView imageView;
	if (vkCreateImageView(_vulkan_device, &viewInfo, nullptr, &imageView) != VK_SUCCESS) {
		throw std::runtime_error("failed to create texture image view!");
	}

	return imageView;
}

bool Vulkan::_create_image_views() {
	_swap_chain_image_views.resize(_swap_chain_images.size());

	for (uint32_t i = 0; i < _swap_chain_images.size(); i++) {
		_swap_chain_image_views[i] = _create_image_view(
			_swap_chain_images[i], _swap_chain_image_format, VK_IMAGE_ASPECT_COLOR_BIT);
	}
	return true;
}

bool Vulkan::_create_descriptor_set_layout() {
	//UNIFORM BUFFER LAYOUT
	VkDescriptorSetLayoutBinding uboLayoutBinding = {};
	uboLayoutBinding.binding = 0;
	uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	uboLayoutBinding.descriptorCount = 1;
	uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT; //VK_SHADER_STAGE_ALL_GRAPHICS
	uboLayoutBinding.pImmutableSamplers = nullptr; // Optional

	//TEXTURE SAMPLER
	VkDescriptorSetLayoutBinding samplerLayoutBinding = {};
	samplerLayoutBinding.binding = 1;
	samplerLayoutBinding.descriptorCount = 1;
	samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	samplerLayoutBinding.pImmutableSamplers = nullptr;
	samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

	std::array<VkDescriptorSetLayoutBinding, 2> bindings = { uboLayoutBinding, samplerLayoutBinding };

	VkDescriptorSetLayoutCreateInfo layoutInfo = {};
	layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
	layoutInfo.pBindings = bindings.data();

	if (vkCreateDescriptorSetLayout(_vulkan_device, &layoutInfo, nullptr, &_descriptor_set_layout) != VK_SUCCESS) {
		throw std::runtime_error("failed to create descriptor set layout!");
	}

	return true;
}

bool Vulkan::_create_graphics_pipeline() {
	//PROGRAMMABLE
	auto vertShaderCode = _read_file("./resources/shader/test/vert.spv");
	auto fragShaderCode = _read_file("./resources/shader/test/frag.spv");

	std::cout << "loading shaders" << std::endl;
	std::cout << "\t" << "vertex shader size: " << vertShaderCode.size() << " bytes" << std::endl;
	std::cout << "\t" << "fragment shader size: " << fragShaderCode.size() << " bytes" << std::endl;

	VkShaderModule vertShaderModule;
	VkShaderModule fragShaderModule;
	_create_shader_module(vertShaderCode, vertShaderModule);
	_create_shader_module(fragShaderCode, fragShaderModule);


	//SHADER STAGE
	VkPipelineShaderStageCreateInfo vertShaderStageInfo = {};
	vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
	vertShaderStageInfo.module = vertShaderModule;
	vertShaderStageInfo.pName = "main";

	VkPipelineShaderStageCreateInfo fragShaderStageInfo = {};
	fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	fragShaderStageInfo.module = fragShaderModule;
	fragShaderStageInfo.pName = "main";

	VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };


	//FIXED FUNCTION
	VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

	auto bindingDescription = Vertex::getBindingDescription();
	auto attributeDescriptions = Vertex::getAttributeDescriptions();

	vertexInputInfo.vertexBindingDescriptionCount = 1;
	vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
	vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
	vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

	VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
	inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	inputAssembly.primitiveRestartEnable = VK_FALSE;


	VkViewport viewport = {};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = (float)_swap_chain_extent.width;
	viewport.height = (float)_swap_chain_extent.height;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;

	VkRect2D scissor = {};
	scissor.offset = { 0, 0 };
	scissor.extent = _swap_chain_extent;

	VkPipelineViewportStateCreateInfo viewportState = {};
	viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportState.viewportCount = 1;
	viewportState.pViewports = &viewport;
	viewportState.scissorCount = 1;
	viewportState.pScissors = &scissor;


	//Rasterizer
	VkPipelineRasterizationStateCreateInfo rasterizer = {};
	rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizer.depthClampEnable = VK_FALSE;

	rasterizer.rasterizerDiscardEnable = VK_FALSE;
	/*
	VK_POLYGON_MODE_FILL
	VK_POLYGON_MODE_LINE	//req. gpu feature
	VK_POLYGON_MODE_POINT	//req. gpu feature
	*/
	rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
	rasterizer.lineWidth = 1.0f;
	rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
	rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
	rasterizer.depthBiasEnable = VK_FALSE;
	rasterizer.depthBiasConstantFactor = 0.0f; // Optional
	rasterizer.depthBiasClamp = 0.0f; // Optional
	rasterizer.depthBiasSlopeFactor = 0.0f; // Optional

	//MULTISAMPLING
	VkPipelineMultisampleStateCreateInfo multisampling = {};
	multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampling.sampleShadingEnable = VK_FALSE;
	multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
	multisampling.minSampleShading = 1.0f; // Optional
	multisampling.pSampleMask = nullptr; /// Optional
	multisampling.alphaToCoverageEnable = VK_FALSE; // Optional
	multisampling.alphaToOneEnable = VK_FALSE; // Optional

	//COLORBLENDING
	VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
	colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;

	//NO BLENDING
	colorBlendAttachment.blendEnable = VK_FALSE;
	colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
	colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
	colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD; // Optional
	colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
	colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
	colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD; // Optional

	//ALPHA BLENDING
	/*
	colorBlendAttachment.blendEnable = VK_TRUE;
	colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
	colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
	colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
	colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
	colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
	colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;
	*/

	VkPipelineColorBlendStateCreateInfo colorBlending = {};
	colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	colorBlending.logicOpEnable = VK_FALSE;
	colorBlending.logicOp = VK_LOGIC_OP_COPY; // Optional
	colorBlending.attachmentCount = 1;
	colorBlending.pAttachments = &colorBlendAttachment;
	colorBlending.blendConstants[0] = 0.0f; // Optional
	colorBlending.blendConstants[1] = 0.0f; // Optional
	colorBlending.blendConstants[2] = 0.0f; // Optional
	colorBlending.blendConstants[3] = 0.0f; // Optional

	//DYNAMIC STATE
	VkDynamicState dynamicStates[] = {
		VK_DYNAMIC_STATE_VIEWPORT,
		VK_DYNAMIC_STATE_LINE_WIDTH
	};

	VkPipelineDynamicStateCreateInfo dynamicState = {};
	dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	dynamicState.dynamicStateCount = 2;
	dynamicState.pDynamicStates = dynamicStates;


	//PIPELINE LAYOUT
	VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = 1;
	pipelineLayoutInfo.pSetLayouts = &_descriptor_set_layout;
	pipelineLayoutInfo.pushConstantRangeCount = 0; // Optional
	pipelineLayoutInfo.pPushConstantRanges = 0; // Optional

	if (vkCreatePipelineLayout(_vulkan_device, &pipelineLayoutInfo, nullptr,
		&_pipeline_layout) != VK_SUCCESS) {
		throw std::runtime_error("failed to create pipeline layout!");
		return false;
	}

	//DEPTH & STENCIL STATE
	VkPipelineDepthStencilStateCreateInfo depthStencil = {};
	depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	depthStencil.depthTestEnable = VK_TRUE;
	depthStencil.depthWriteEnable = VK_TRUE;
	depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
	depthStencil.depthBoundsTestEnable = VK_FALSE;
	depthStencil.minDepthBounds = 0.0f; // Optional
	depthStencil.maxDepthBounds = 1.0f; // Optional
	depthStencil.stencilTestEnable = VK_FALSE;
	depthStencil.front = {}; // Optional
	depthStencil.back = {}; // Optional

	//PIPELINE
	VkGraphicsPipelineCreateInfo pipelineInfo = {};
	pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineInfo.stageCount = 2;
	pipelineInfo.pStages = shaderStages;
	//fixed function
	pipelineInfo.pVertexInputState = &vertexInputInfo;
	pipelineInfo.pInputAssemblyState = &inputAssembly;
	pipelineInfo.pViewportState = &viewportState;
	pipelineInfo.pRasterizationState = &rasterizer;
	pipelineInfo.pMultisampleState = &multisampling;
	pipelineInfo.pDepthStencilState = nullptr; // Optional
	pipelineInfo.pColorBlendState = &colorBlending;
	pipelineInfo.pDynamicState = nullptr; // Optional
	//pipeline layout
	pipelineInfo.layout = _pipeline_layout;
	//render pass
	pipelineInfo.renderPass = _render_pass;
	pipelineInfo.subpass = 0;
	//if this pilepline is a derived pipeline we need to set the following two ..
	pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // Optional
	pipelineInfo.basePipelineIndex = -1; // Optional
	//depth & stencil
	pipelineInfo.pDepthStencilState = &depthStencil;

	if (vkCreateGraphicsPipelines(_vulkan_device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &_graphics_pipeline) != VK_SUCCESS) {
		throw std::runtime_error("failed to create graphics pipeline!");
		return false;
	}

	vkDestroyShaderModule(_vulkan_device, vertShaderModule, nullptr);
	vkDestroyShaderModule(_vulkan_device, fragShaderModule, nullptr);

	return true;
}

std::vector<char> Vulkan::_read_file(const std::string& filename) {
	std::ifstream file(filename, std::ios::ate | std::ios::binary);

	if (!file.is_open()) {
		throw std::runtime_error("readFile: failed to open file: " + filename);
	}

	size_t fileSize = (size_t)file.tellg();
	std::vector<char> buffer(fileSize);

	file.seekg(0);
	file.read(buffer.data(), fileSize);
	file.close();

	return buffer;
}

bool Vulkan::_create_shader_module(const std::vector<char>& code, VkShaderModule& shaderModule) {
	VkShaderModuleCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = code.size();

	std::vector<uint32_t> codeAligned(code.size() / sizeof(uint32_t) + 1);
	memcpy(codeAligned.data(), code.data(), code.size());
	createInfo.pCode = codeAligned.data();

	if (vkCreateShaderModule(_vulkan_device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
		throw std::runtime_error("failed to create shader module!");
		return false;
	}
	return true;
}

bool Vulkan::_create_render_pass() {

	VkAttachmentDescription depthAttachment = {};
	depthAttachment.format = _find_depth_format();
	depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	VkAttachmentReference depthAttachmentRef = {};
	depthAttachmentRef.attachment = 1;
	depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;


	VkAttachmentDescription colorAttachment = {};
	colorAttachment.format = _swap_chain_image_format;
	colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	//color + depth
	colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	//stencil
	colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;


	//SUBPASSES + ATTACHMENT REFs
	VkAttachmentReference colorAttachmentRef = {};
	colorAttachmentRef.attachment = 0; //layout(location = 0)
	colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkSubpassDescription subpass = {}; //subpass at index 0
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &colorAttachmentRef;
	subpass.pDepthStencilAttachment = &depthAttachmentRef;

	//SUBPASS DEPENDENCY
	VkSubpassDependency dependency = {};
	dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	dependency.dstSubpass = 0; //subpass index for above subpass...

	dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.srcAccessMask = 0;

	dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

	std::array<VkAttachmentDescription, 2> attachments = { colorAttachment, depthAttachment };

	//create render pass
	VkRenderPassCreateInfo renderPassInfo = {};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
	renderPassInfo.pAttachments = attachments.data();
	renderPassInfo.subpassCount = 1;
	renderPassInfo.pSubpasses = &subpass;
	renderPassInfo.dependencyCount = 1;
	renderPassInfo.pDependencies = &dependency;

	if (vkCreateRenderPass(_vulkan_device, &renderPassInfo, nullptr, &_render_pass) != VK_SUCCESS) {
		throw std::runtime_error("failed to create render pass!");
		return false;
	}
	return true;
}

bool Vulkan::_create_frame_buffers() {
	_swap_chain_framebuffers.resize(_swap_chain_image_views.size());

	for (size_t i = 0; i < _swap_chain_image_views.size(); i++) {
		std::array<VkImageView, 2> attachments = {
			_swap_chain_image_views[i],
			_depth_image_view
		};

		VkFramebufferCreateInfo framebufferInfo = {};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = _render_pass;
		framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
		framebufferInfo.pAttachments = attachments.data();
		framebufferInfo.width = _swap_chain_extent.width;
		framebufferInfo.height = _swap_chain_extent.height;
		framebufferInfo.layers = 1;

		if (vkCreateFramebuffer(_vulkan_device, &framebufferInfo, nullptr, &_swap_chain_framebuffers[i]) != VK_SUCCESS) {
			throw std::runtime_error("failed to create framebuffer!");
			return false;
		}
	}

	return true;
}

bool Vulkan::_create_command_pool() {
	QueueFamilyIndices queueFamilyIndices = _find_queue_families(_physical_device);

	VkCommandPoolCreateInfo poolInfo = {};
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily;
	poolInfo.flags = 0; // Optional

	if (vkCreateCommandPool(_vulkan_device, &poolInfo, nullptr, &_command_pool) != VK_SUCCESS) {
		throw std::runtime_error("failed to create command pool!");
		return false;
	}

	return true;
}


VkFormat Vulkan::_find_supported_format(
	const std::vector<VkFormat>& candidates,
	VkImageTiling tiling,
	VkFormatFeatureFlags features)
{
	for (VkFormat format : candidates) {
		VkFormatProperties props;
		vkGetPhysicalDeviceFormatProperties(_physical_device, format, &props);

		if (tiling == VK_IMAGE_TILING_LINEAR &&
			(props.linearTilingFeatures & features) == features)
		{
			return format;
		}
		else if (tiling == VK_IMAGE_TILING_OPTIMAL &&
			(props.optimalTilingFeatures & features) == features)
		{
			return format;
		}
	}

	throw std::runtime_error("failed to find supported format!");
}

VkFormat Vulkan::_find_depth_format() {
	return _find_supported_format(
		{ VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
		VK_IMAGE_TILING_OPTIMAL,
		VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
	);
}

bool Vulkan::_has_stencil_component(VkFormat format) {
	return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
}

bool Vulkan::_create_depth_resources() {
	VkFormat depthFormat = _find_depth_format();
	_create_image(
		_swap_chain_extent.width,
		_swap_chain_extent.height,
		depthFormat,
		VK_IMAGE_TILING_OPTIMAL,
		VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		_depth_image,
		_depth_image_memory);
	_depth_image_view = _create_image_view(_depth_image, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT);
	
	_transition_image_layout(
		_depth_image,
		depthFormat,
		VK_IMAGE_LAYOUT_UNDEFINED,
		VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
	return true;
}

bool Vulkan::_create_image(
	uint32_t width, uint32_t height, VkFormat format,
	VkImageTiling tiling, VkImageUsageFlags usage,
	VkMemoryPropertyFlags properties, VkImage& image,
	VkDeviceMemory& imageMemory) {

	VkImageCreateInfo imageInfo = {};
	imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	imageInfo.imageType = VK_IMAGE_TYPE_2D;
	imageInfo.extent.width = width;
	imageInfo.extent.height = height;
	imageInfo.extent.depth = 1;
	imageInfo.mipLevels = 1;
	imageInfo.arrayLayers = 1;
	imageInfo.format = format;
	imageInfo.tiling = tiling;
	imageInfo.initialLayout = VK_IMAGE_LAYOUT_PREINITIALIZED;
	imageInfo.usage = usage;
	imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
	imageInfo.flags = 0; // Optional

	if (vkCreateImage(_vulkan_device, &imageInfo, nullptr, &image) != VK_SUCCESS) {
		throw std::runtime_error("failed to create image!");
	}

	VkMemoryRequirements memRequirements;
	vkGetImageMemoryRequirements(_vulkan_device, image, &memRequirements);

	VkMemoryAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = _find_memory_type(memRequirements.memoryTypeBits, properties);

	if (vkAllocateMemory(_vulkan_device, &allocInfo, nullptr, &imageMemory) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate image memory!");
	}

	vkBindImageMemory(_vulkan_device, image, imageMemory, 0);

	return true;
}

bool Vulkan::_create_texture_image() {
	int texWidth, texHeight, texChannels;
	stbi_uc* pixels = stbi_load(
		"resources/textures/texture.jpg",
		&texWidth, &texHeight,
		&texChannels,
		STBI_rgb_alpha);
	VkDeviceSize imageSize = texWidth * texHeight * 4;

	if (!pixels) {
		throw std::runtime_error("failed to load texture image!");
	}

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	_create_buffer(
		imageSize,
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		stagingBuffer, stagingBufferMemory);

	void* data;
	vkMapMemory(_vulkan_device, stagingBufferMemory, 0, imageSize, 0, &data);
	memcpy(data, pixels, static_cast<size_t>(imageSize));
	vkUnmapMemory(_vulkan_device, stagingBufferMemory);

	stbi_image_free(pixels);

	_create_image(
		texWidth, texHeight,
		VK_FORMAT_R8G8B8A8_UNORM,
		VK_IMAGE_TILING_OPTIMAL,
		VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		_texture_image,
		_texture_image_memory);

	_transition_image_layout(
		_texture_image,
		VK_FORMAT_R8G8B8A8_UNORM,
		VK_IMAGE_LAYOUT_PREINITIALIZED,
		VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
	_copy_buffer_to_image(
		stagingBuffer,
		_texture_image,
		static_cast<uint32_t>(texWidth),
		static_cast<uint32_t>(texHeight));
	//prepare for sampling in the shader
	_transition_image_layout(
		_texture_image,
		VK_FORMAT_R8G8B8A8_UNORM,
		VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
		VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

	vkDestroyBuffer(_vulkan_device, stagingBuffer, nullptr);
	vkFreeMemory(_vulkan_device, stagingBufferMemory, nullptr);

	return true;
}

bool Vulkan::_transition_image_layout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout) {
	VkCommandBuffer commandBuffer = _begin_single_time_commands();

	VkImageMemoryBarrier barrier = {};
	barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	barrier.oldLayout = oldLayout; //VK_IMAGE_LAYOUT_UNDEFINED (if old layout doesnt matter...)
	barrier.newLayout = newLayout;

	//for queue family ownership transfer (not used here...)
	barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

	barrier.image = image;
	barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	barrier.subresourceRange.baseMipLevel = 0;
	barrier.subresourceRange.levelCount = 1;
	barrier.subresourceRange.baseArrayLayer = 0;
	barrier.subresourceRange.layerCount = 1;

	if (newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
		barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;

		if (_has_stencil_component(format)) {
			barrier.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
		}
	}
	else {
		barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	}

	if (oldLayout == VK_IMAGE_LAYOUT_PREINITIALIZED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL) {
		barrier.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT;
		barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
	}
	else if (oldLayout == VK_IMAGE_LAYOUT_PREINITIALIZED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
		barrier.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT;
		barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
	}
	else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
		barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
	}
	else if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
		barrier.srcAccessMask = 0;
		barrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
	}
	else {
		throw std::invalid_argument("unsupported layout transition!");
	}

	vkCmdPipelineBarrier(
		commandBuffer,
		VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
		0,
		0, nullptr,
		0, nullptr,
		1, &barrier
	);

	_end_single_time_commands(commandBuffer);
	return true;
}

bool Vulkan::_copy_buffer_to_image(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height) {
	VkCommandBuffer commandBuffer = _begin_single_time_commands();

	VkBufferImageCopy region = {};
	region.bufferOffset = 0;
	region.bufferRowLength = 0;
	region.bufferImageHeight = 0;

	region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	region.imageSubresource.mipLevel = 0;
	region.imageSubresource.baseArrayLayer = 0;
	region.imageSubresource.layerCount = 1;

	region.imageOffset = { 0, 0, 0 };
	region.imageExtent = {
		width,
		height,
		1
	};

	vkCmdCopyBufferToImage(
		commandBuffer,
		buffer,
		image,
		VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
		1,
		&region
	);

	_end_single_time_commands(commandBuffer);
	return true;
}

bool Vulkan::_create_texture_image_view() {
	_texture_image_view = _create_image_view(
		_texture_image, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_ASPECT_COLOR_BIT);
	return true;
}

bool Vulkan::_create_texture_sampler() {
	VkSamplerCreateInfo samplerInfo = {};
	samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	samplerInfo.magFilter = VK_FILTER_LINEAR;
	samplerInfo.minFilter = VK_FILTER_LINEAR;
	samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.anisotropyEnable = VK_TRUE;
	samplerInfo.maxAnisotropy = 16;
	samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
	samplerInfo.unnormalizedCoordinates = VK_FALSE;
	samplerInfo.compareEnable = VK_FALSE;
	samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;

	samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
	samplerInfo.mipLodBias = 0.0f;
	samplerInfo.minLod = 0.0f;
	samplerInfo.maxLod = 0.0f;

	if (vkCreateSampler(_vulkan_device, &samplerInfo, nullptr, &_texture_sampler) != VK_SUCCESS) {
		throw std::runtime_error("failed to create texture sampler!");
	}


	return true;
}

bool Vulkan::_create_descriptor_pool() {
	std::array<VkDescriptorPoolSize, 2> poolSizes = {};
	poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	poolSizes[0].descriptorCount = 1;
	poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	poolSizes[1].descriptorCount = 1;

	VkDescriptorPoolCreateInfo poolInfo = {};
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
	poolInfo.pPoolSizes = poolSizes.data();
	poolInfo.maxSets = 1;

	if (vkCreateDescriptorPool(_vulkan_device, &poolInfo, nullptr, &_descriptor_pool) != VK_SUCCESS) {
		throw std::runtime_error("failed to create descriptor pool!");
	}

	return true;
}

bool Vulkan::_create_descriptor_set() {
	VkDescriptorSetLayout layouts[] = { _descriptor_set_layout };
	VkDescriptorSetAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = _descriptor_pool;
	allocInfo.descriptorSetCount = 1;
	allocInfo.pSetLayouts = layouts;

	if (vkAllocateDescriptorSets(_vulkan_device, &allocInfo, &_descriptor_set) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate descriptor set!");
	}

	VkDescriptorBufferInfo bufferInfo = {};
	bufferInfo.buffer = _uniform_buffer;
	bufferInfo.offset = 0;
	bufferInfo.range = sizeof(UniformBufferObject);

	VkDescriptorImageInfo imageInfo = {};
	imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	imageInfo.imageView = _texture_image_view;
	imageInfo.sampler = _texture_sampler;

	std::array<VkWriteDescriptorSet, 2> descriptorWrites = {};

	descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	descriptorWrites[0].dstSet = _descriptor_set;
	descriptorWrites[0].dstBinding = 0;
	descriptorWrites[0].dstArrayElement = 0;
	descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	descriptorWrites[0].descriptorCount = 1;
	descriptorWrites[0].pBufferInfo = &bufferInfo;

	descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	descriptorWrites[1].dstSet = _descriptor_set;
	descriptorWrites[1].dstBinding = 1;
	descriptorWrites[1].dstArrayElement = 0;
	descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	descriptorWrites[1].descriptorCount = 1;
	descriptorWrites[1].pImageInfo = &imageInfo;

	vkUpdateDescriptorSets(_vulkan_device, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);

	return true;
}

bool Vulkan::_create_buffer(
	VkDeviceSize size, VkBufferUsageFlags usage,
	VkMemoryPropertyFlags properties, VkBuffer& buffer,
	VkDeviceMemory& bufferMemory)
{
	VkBufferCreateInfo bufferInfo = {};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = size;
	bufferInfo.usage = usage;
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	if (vkCreateBuffer(_vulkan_device, &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
		throw std::runtime_error("failed to create buffer!");
	}

	VkMemoryRequirements memRequirements;
	vkGetBufferMemoryRequirements(_vulkan_device, buffer, &memRequirements);

	VkMemoryAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = _find_memory_type(memRequirements.memoryTypeBits, properties);

	if (vkAllocateMemory(_vulkan_device, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate buffer memory!");
	}

	vkBindBufferMemory(_vulkan_device, buffer, bufferMemory, 0);
	return true;
}

bool Vulkan::_copy_buffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) {
	VkCommandBuffer commandBuffer = _begin_single_time_commands();

	VkBufferCopy copyRegion = {};
	copyRegion.srcOffset = 0; // Optional
	copyRegion.dstOffset = 0; // Optional
	copyRegion.size = size;
	vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

	_end_single_time_commands(commandBuffer);
	
	return true;
}


bool Vulkan::_create_vertex_buffer() {
	VkDeviceSize bufferSize = sizeof(_vertices[0]) * _vertices.size();

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	_create_buffer(
		bufferSize,
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		stagingBuffer, stagingBufferMemory);

	void* data;
	vkMapMemory(_vulkan_device, stagingBufferMemory, 0, bufferSize, 0, &data);
	memcpy(data, _vertices.data(), (size_t)bufferSize);
	vkUnmapMemory(_vulkan_device, stagingBufferMemory);

	_create_buffer(
		bufferSize,
		VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		_vertex_buffer, _vertex_buffer_memory);

	_copy_buffer(stagingBuffer, _vertex_buffer, bufferSize);

	vkDestroyBuffer(_vulkan_device, stagingBuffer, nullptr);
	vkFreeMemory(_vulkan_device, stagingBufferMemory, nullptr);

	return true;
}

bool Vulkan::_create_index_buffer() {
	VkDeviceSize bufferSize = sizeof(_indices[0]) * _indices.size();

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	_create_buffer(
		bufferSize,
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		stagingBuffer, stagingBufferMemory);

	void* data;
	vkMapMemory(_vulkan_device, stagingBufferMemory, 0, bufferSize, 0, &data);
	memcpy(data, _indices.data(), (size_t)bufferSize);
	vkUnmapMemory(_vulkan_device, stagingBufferMemory);

	_create_buffer(
		bufferSize,
		VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		_index_buffer, _index_buffer_memory);

	_copy_buffer(stagingBuffer, _index_buffer, bufferSize);

	vkDestroyBuffer(_vulkan_device, stagingBuffer, nullptr);
	vkFreeMemory(_vulkan_device, stagingBufferMemory, nullptr);

	return true;
}

bool Vulkan::_create_uniform_buffer() {
	VkDeviceSize bufferSize = sizeof(UniformBufferObject);
	_create_buffer(
		bufferSize,
		VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		_uniform_buffer, _uniform_buffer_memory);
	return true;
}

uint32_t Vulkan::_find_memory_type(uint32_t typeFilter, VkMemoryPropertyFlags properties) {
	VkPhysicalDeviceMemoryProperties memProperties;
	vkGetPhysicalDeviceMemoryProperties(_physical_device, &memProperties);

	for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
		if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
			return i;
		}
	}

	throw std::runtime_error("failed to find suitable memory type!");
}

VkCommandBuffer Vulkan::_begin_single_time_commands() {
	VkCommandBufferAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandPool = _command_pool;
	allocInfo.commandBufferCount = 1;

	VkCommandBuffer commandBuffer;
	vkAllocateCommandBuffers(_vulkan_device, &allocInfo, &commandBuffer);

	VkCommandBufferBeginInfo beginInfo = {};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	vkBeginCommandBuffer(commandBuffer, &beginInfo);

	return commandBuffer;
}

void Vulkan::_end_single_time_commands(VkCommandBuffer commandBuffer) {
	vkEndCommandBuffer(commandBuffer);

	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffer;

	vkQueueSubmit(_graphics_queue, 1, &submitInfo, VK_NULL_HANDLE);
	vkQueueWaitIdle(_graphics_queue);

	vkFreeCommandBuffers(_vulkan_device, _command_pool, 1, &commandBuffer);
}

bool Vulkan::_create_command_buffers() {
	_command_buffers.resize(_swap_chain_framebuffers.size());

	VkCommandBufferAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = _command_pool;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = (uint32_t)_command_buffers.size();

	if (vkAllocateCommandBuffers(_vulkan_device, &allocInfo, _command_buffers.data()) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate command buffers!");
		return false;
	}

	//record command buffers
	for (size_t i = 0; i < _command_buffers.size(); i++) {
		VkCommandBufferBeginInfo beginInfo = {};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
		beginInfo.pInheritanceInfo = nullptr; // Optional

		vkBeginCommandBuffer(_command_buffers[i], &beginInfo);


		//render pass
		VkRenderPassBeginInfo renderPassInfo = {};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		//attachments
		renderPassInfo.renderPass = _render_pass;
		renderPassInfo.framebuffer = _swap_chain_framebuffers[i];
		//size to render
		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = _swap_chain_extent;
		//clear color
		std::array<VkClearValue, 2> clearValues = {};
		clearValues[0].color = { 0.0f, 0.0f, 0.0f, 1.0f };
		clearValues[1].depthStencil = { 1.0f, 0 };
		renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
		renderPassInfo.pClearValues = clearValues.data();
		//begin render pass
		vkCmdBeginRenderPass(_command_buffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
		//bind pipeline
		vkCmdBindPipeline(_command_buffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, _graphics_pipeline);
		//DRAW COMMAND !!!!

		VkBuffer vertexBuffers[] = { _vertex_buffer };
		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(_command_buffers[i], 0, 1, vertexBuffers, offsets);
		vkCmdBindIndexBuffer(_command_buffers[i], _index_buffer, 0, VK_INDEX_TYPE_UINT32);
		vkCmdBindDescriptorSets(
			_command_buffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS,
			_pipeline_layout, 0, 1, &_descriptor_set, 0, nullptr);

		//vkCmdDraw(_command_buffers[i], static_cast<uint32_t>(_vertices.size()), 1, 0, 0);
		vkCmdDrawIndexed(_command_buffers[i], static_cast<uint32_t>(_indices.size()), 1, 0, 0, 0);

		vkCmdEndRenderPass(_command_buffers[i]);

		if (vkEndCommandBuffer(_command_buffers[i]) != VK_SUCCESS) {
			throw std::runtime_error("failed to record command buffer!");
			return false;
		}
	}

	return true;
}

bool Vulkan::_create_semaphores() {
	VkSemaphoreCreateInfo semaphoreInfo = {};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	if (vkCreateSemaphore(_vulkan_device, &semaphoreInfo, nullptr, &_image_available_semaphore) != VK_SUCCESS ||
		vkCreateSemaphore(_vulkan_device, &semaphoreInfo, nullptr, &_render_finished_semaphore) != VK_SUCCESS) {

		throw std::runtime_error("failed to create semaphores!");
		return false;
	}

	return true;
}

bool Vulkan::_post_init() {
	return true;
}

void Vulkan::updateUniformBuffer() {
	static auto startTime = std::chrono::high_resolution_clock::now();
	auto currentTime = std::chrono::high_resolution_clock::now();
	float time = std::chrono::duration_cast<std::chrono::milliseconds>(
		currentTime - startTime).count() / 1000.0f;

	UniformBufferObject ubo = {};
	ubo.model = glm::rotate(
		glm::mat4(),
		time * glm::radians(90.0f),
		glm::vec3(0.0f, 0.0f, 1.0f));

	ubo.view = glm::lookAt(
		glm::vec3(2.0f, 2.0f, 2.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 1.0f));

	ubo.proj = glm::perspective(
		glm::radians(45.0f),
		_swap_chain_extent.width / (float)_swap_chain_extent.height,
		0.1f, 10.0f);

	ubo.proj[1][1] *= -1; //flip

	void* data;
	vkMapMemory(_vulkan_device, _uniform_buffer_memory, 0, sizeof(ubo), 0, &data);
	memcpy(data, &ubo, sizeof(ubo));
	vkUnmapMemory(_vulkan_device, _uniform_buffer_memory);
}

void Vulkan::drawFrame() {
	//get image index from swap chain
	uint32_t imageIndex;
	VkResult result = vkAcquireNextImageKHR(
		_vulkan_device,
		_swap_chain,
		std::numeric_limits<uint64_t>::max(),
		_image_available_semaphore,
		VK_NULL_HANDLE,
		&imageIndex);

	if(result == VK_ERROR_OUT_OF_DATE_KHR) {
		_recreate_swap_chain();
		return;
	}
	 else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
		 throw std::runtime_error("failed to acquire swap chain image!");
	 }

	//submit command buffer
	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	VkSemaphore waitSemaphores[] = { _image_available_semaphore };
	VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;
	//specify command buffer
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &_command_buffers[imageIndex];


	VkSemaphore signalSemaphores[] = { _render_finished_semaphore };
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;
	if (vkQueueSubmit(_graphics_queue, 1, &submitInfo, VK_NULL_HANDLE) != VK_SUCCESS) {
		throw std::runtime_error("failed to submit draw command buffer!");
	}


	//PRESENTATION
	VkPresentInfoKHR presentInfo = {};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphores;

	VkSwapchainKHR swapChains[] = { _swap_chain };
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChains;
	presentInfo.pImageIndices = &imageIndex;

	presentInfo.pResults = nullptr; // Optional

	result = vkQueuePresentKHR(_present_queue, &presentInfo);

	if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
		_recreate_swap_chain();
	}
	else if (result != VK_SUCCESS) {
		throw std::runtime_error("failed to present swap chain image!");
	}

	vkQueueWaitIdle(_present_queue);
}

void Vulkan::windowResize(int width, int height) {
	std::cout << "resizing window: w" << width << " h" << height << std::endl;
	_window_height = height;
	_window_width = width;
	_recreate_swap_chain();
}

bool Vulkan::cleanUp() {
	vkDeviceWaitIdle(_vulkan_device);

	_clean_up_swap_chain();

	vkDestroySampler(_vulkan_device, _texture_sampler, nullptr);
	vkDestroyImageView(_vulkan_device, _texture_image_view, nullptr);

	vkDestroyImage(_vulkan_device, _texture_image, nullptr);
	vkFreeMemory(_vulkan_device, _texture_image_memory, nullptr);

	vkDestroyDescriptorPool(_vulkan_device, _descriptor_pool, nullptr);
	vkDestroyDescriptorSetLayout(_vulkan_device, _descriptor_set_layout, nullptr);
	vkDestroyBuffer(_vulkan_device, _uniform_buffer, nullptr);
	vkFreeMemory(_vulkan_device, _uniform_buffer_memory, nullptr);

	vkDestroyBuffer(_vulkan_device, _index_buffer, nullptr);
	vkFreeMemory(_vulkan_device, _index_buffer_memory, nullptr);

	vkDestroyBuffer(_vulkan_device, _vertex_buffer, nullptr);
	vkFreeMemory(_vulkan_device, _vertex_buffer_memory, nullptr);

	vkDestroySemaphore(_vulkan_device, _render_finished_semaphore, nullptr);
	vkDestroySemaphore(_vulkan_device, _image_available_semaphore, nullptr);

	vkDestroyCommandPool(_vulkan_device, _command_pool, nullptr);

	vkDestroyDevice(_vulkan_device, nullptr);
	_destroy_debug_report_callback_EXT(_vulkan_instance, _debug_callback_instance, nullptr);
	vkDestroySurfaceKHR(_vulkan_instance, _vulkan_surface, nullptr);
	vkDestroyInstance(_vulkan_instance, nullptr);

	return true;
}