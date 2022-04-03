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
	bool preInit();

//VULKAN INIT
	bool initVulkan();
//VULKAN INSTANCE
	bool createInstance();
//VULKAN VALIDATION
	bool checkValidationLayerSupport();
	std::vector<const char*> getRequiredExtensions();
	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
		VkDebugReportFlagsEXT flags,
		VkDebugReportObjectTypeEXT objType,
		uint64_t obj,
		size_t location,
		int32_t code,
		const char* layerPrefix,
		const char* msg,
		void* userData);
	void setupDebugCallback();
	static VkResult createDebugReportCallbackEXT(
		VkInstance instance,
		const VkDebugReportCallbackCreateInfoEXT* pCreateInfo,
		const VkAllocationCallbacks* pAllocator,
		VkDebugReportCallbackEXT* pCallback);
	static void destroyDebugReportCallbackEXT(
		VkInstance instance,
		VkDebugReportCallbackEXT callback,
		const VkAllocationCallbacks* pAllocator);
//VULKAN PHYSICAL DEVICE
	bool pickPhysicalDevice();
	bool isDeviceSuitable(VkPhysicalDevice device);
	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
//VULKAN LOGICAL DEVICE
	bool createLogicalDevice();
//VULKAN SURFACE
	bool createSurface();
//VULKAN DEVICE EXTENSION SUPPORT (SWAPCHAIN, ...)
	bool checkDeviceExtensionSupport(VkPhysicalDevice device);
	SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
//VULKAN SWAPCHAIN CONFIG
	VkSurfaceFormatKHR chooseSwapSurfaceFormat(
		const std::vector<VkSurfaceFormatKHR>& availableFormats);
	VkPresentModeKHR chooseSwapPresentMode(
		const std::vector<VkPresentModeKHR> availablePresentModes);
	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
	bool createSwapChain();
	bool recreateSwapChain();
	bool cleanUpSwapChain();
//VULKAN IMAGE VIEWS
	VkImageView createImageView(
		VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);
	bool createImageViews();
//VULKAN DESCRIPTORSETS
	bool createDescriptorSetLayout();
//VULKAN GFX PIPELINES
	bool createGraphicsPipeline();
	static std::vector<char> readFile(const std::string& filename);
	bool createShaderModule(const std::vector<char>& code, VkShaderModule& shaderModule);
//VULKAN RENDER PASS
	bool createRenderPass();
//VULKAN FRAMEBFFERS
	bool createFrameBuffers();
//VULKAN COMMAND POOL
	bool createCommandPool();
//VULKAN SINGLE TIME COMMANDS
	VkCommandBuffer beginSingleTimeCommands();
	void endSingleTimeCommands(VkCommandBuffer commandBuffer);
//VULKAN DEPTH RES
	VkFormat findSupportedFormat(
		const std::vector<VkFormat>& candidates,
		VkImageTiling tiling,
		VkFormatFeatureFlags features);
	VkFormat findDepthFormat();
	bool hasStencilComponent(VkFormat format);
	bool createDepthResources();
//VULKAN TEXTURE IMAGE
	bool createImage(
		uint32_t width, uint32_t height, VkFormat format,
		VkImageTiling tiling, VkImageUsageFlags usage,
		VkMemoryPropertyFlags properties, VkImage& image,
		VkDeviceMemory& imageMemory);
	bool createTextureImage();
	bool transitionImageLayout(
		VkImage image, VkFormat format,
		VkImageLayout oldLayout, VkImageLayout newLayout);
	bool copyBufferToImage(
		VkBuffer buffer, VkImage image,
		uint32_t width, uint32_t height);
	bool createTextureImageView();
	bool createTextureSampler();
//VULKAN DESCRIPTOR POOL
	bool createDescriptorPool();
	bool createDescriptorSet();
//VULKAN BUFFER
	bool createBuffer(
		VkDeviceSize size, VkBufferUsageFlags usage,
		VkMemoryPropertyFlags properties, VkBuffer& buffer,
		VkDeviceMemory& bufferMemory);
	bool copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
//VULKAN BUFFER
	bool createVertexBuffer();
	bool createIndexBuffer();
	bool createUniformBuffer();
	uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
//VULKAN COMMAND BUFFERS
	bool createCommandBuffers();
//VULKAN SEMAPHORES
	bool createSemaphores();
	bool postInit();


	//if in debug
	bool						isDebug;

	GLFWwindow *				mGlfwWindow;
	int							mWindowHeight;
	int							mWindowWidth;

	VkInstance					mVulkanInstance;
	VkDebugReportCallbackEXT	mDebugCallbackInstance;

	VkSurfaceKHR				mVulkanSurface;

	VkPhysicalDevice			mPhysicalDevice = VK_NULL_HANDLE;
	VkDevice					mVulkanDevice; //logical device

	VkQueue						mGraphicsQueue;
	VkQueue						mPresentQueue;

	VkSwapchainKHR				mSwapChain;

	std::vector<VkImage>		mSwapChainImages;

	VkFormat					mSwapChainImageFormat;
	VkExtent2D					mSwapChainExtent;

	std::vector<VkImageView>	mSwapChainImageViews;

	VkImage						mDepthImage;
	VkDeviceMemory				mDepthImageMemory;
	VkImageView					mDepthImageView;

	VkDescriptorSetLayout		mDescriptorSetLayout;
	VkPipelineLayout			mPipelineLayout;

	VkRenderPass				mRenderPass;

	VkPipeline					mGraphicsPipeline;

	std::vector<VkFramebuffer>	mSwapChainFrameBuffers;

	VkCommandPool				mCommandPool;

	VkImage						mTextureImage;
	VkDeviceMemory				mTextureImageMemory;
	VkImageView					mTextureImageView;
	VkSampler					mTextureSampler;

	VkDescriptorPool			mDescriptorPool;
	VkDescriptorSet				mDescriptorSet;

	VkBuffer					mVertexBuffer;
	VkDeviceMemory				mVertexBufferMemory;
	VkBuffer					mIndexBuffer;
	VkDeviceMemory				mIndexBufferMemory;
	VkBuffer					mUniformBuffer;
	VkDeviceMemory				mUniformBufferMemory;

	std::vector<VkCommandBuffer> mCommandBuffers;

	//RENDERING
	VkSemaphore mImageAvailableSemaphore;
	VkSemaphore mRenderFinishedSemaphore;

	const std::vector<Vertex> mVertices = {
		{ { -0.5f, -0.5f, 0.0f }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f } },
		{ {  0.5f, -0.5f, 0.0f }, { 0.0f, 1.0f, 0.0f }, { 1.0f, 0.0f } },
		{ {  0.5f,  0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f } },
		{ { -0.5f,  0.5f, 0.0f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f } },

		{ { -0.5f, -0.5f, -0.5f }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f } },
		{ {  0.5f, -0.5f, -0.5f }, { 0.0f, 1.0f, 0.0f }, { 1.0f, 0.0f } },
		{ {  0.5f,  0.5f, -0.5f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f } },
		{ { -0.5f,  0.5f, -0.5f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f } }
	};
	
	const std::vector<uint32_t> mIndices = {
		0, 1, 2, 2, 3, 0,
		4, 5, 6, 6, 7, 4
	};

	//Test model loading...
	const std::string MODEL_PATH = "models/chalet.obj";
	const std::string TEXTURE_PATH = "textures/chalet.jpg";
};

#endif // VULKAN_H
