#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <optional>
#include <memory>

namespace FVulkanEngine
{
	class Device
	{
	public:
		struct QueueFamilyIndices
		{
			std::optional<uint32_t> graphicsFamily;
			std::optional<uint32_t> presentFamily;

			bool isComplete() { return graphicsFamily.has_value() && presentFamily.has_value(); }
		};
	public:
		struct SwapChainSupportDetails
		{
			VkSurfaceCapabilitiesKHR capabilities;
			std::vector<VkSurfaceFormatKHR> formats;
			std::vector<VkPresentModeKHR> presentModes;
		};
	public:
		Device(const VkInstance& instance, bool enableValidationLayers, const std::vector<const char*>& validationLayers, const VkSurfaceKHR& surface);
		~Device();
		Device(const Device&) = delete;
		Device& operator=(const Device&) = delete;
	public:
		void createPhysicalDevice(const VkInstance& instance);
		void createLogicalDevice(bool enableValidationLayers, const std::vector<const char*>& validationLayers);
		SwapChainSupportDetails getDeviceSwapChainSupportDetails() const;
		QueueFamilyIndices findPhysicalQueueFamilies() const;
		const VkDevice& getDevice() const;
		void createCommandPool();
		void createCommandBuffers();
		std::vector<VkCommandBuffer>& getCommandBuffer();
		VkQueue& getGraphicsQueue();
		VkQueue& getPresentQueue();
	private:
		void pickPhysicalDevice(const VkInstance& instance);
		bool isDeviceSuitable(VkPhysicalDevice device);
		QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device) const;
		bool checkDeviceExtensionSupport(VkPhysicalDevice device);
		SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device) const;
	private:
		VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
		VkPhysicalDeviceProperties properties;
		VkDevice device;
		VkQueue graphicsQueue;
		VkQueue presentQueue;
		SwapChainSupportDetails deviceSwapChainDetails;
		const VkSurfaceKHR& pSurface;
		const std::vector<const char*> deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
		VkCommandPool commandPool;
		std::vector<VkCommandBuffer> commandBuffers;
	};
}