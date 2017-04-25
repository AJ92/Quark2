#include "vulkan.h"


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

	if (vkCreateInstance(&createInfo, nullptr, _vulkan_instance.replace()) != VK_SUCCESS) {
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

	if (_create_debug_report_callback_EXT(_vulkan_instance, &createInfo, nullptr, _debug_callback_instance.replace()) != VK_SUCCESS) {
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

	//check if device has what it takes to be a great vulkan device!
	return indices.isComplete() && extensionsSupported && swapChainAdequate;
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

	if (vkCreateDevice(_physical_device, &createInfo, nullptr, _vulkan_device.replace()) != VK_SUCCESS) {
		throw std::runtime_error("failed to create logical device!");
	}

	vkGetDeviceQueue(_vulkan_device, indices.graphicsFamily, 0, &_graphics_queue);
	vkGetDeviceQueue(_vulkan_device, indices.presentFamily, 0, &_present_queue);

	return true;
}

bool Vulkan::_create_surface() {
	if (glfwCreateWindowSurface(_vulkan_instance, _glfw_window, nullptr, _vulkan_surface.replace()) != VK_SUCCESS) {
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

	createInfo.oldSwapchain = VK_NULL_HANDLE; //handle to old swapchain... (todo...)

	if (vkCreateSwapchainKHR(_vulkan_device, &createInfo, nullptr, _swap_chain.replace()) != VK_SUCCESS) {
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

bool Vulkan::_post_init() {
	return true;
}