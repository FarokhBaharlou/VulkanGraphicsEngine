#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <optional>
#include <memory>

namespace FVulkanEngine
{
	class Device
	{
	private:
		struct QueueFamilyIndices
		{
			std::optional<uint32_t> graphicsFamily;
			std::optional<uint32_t> presentFamily;

			bool isComplete() { return graphicsFamily.has_value() && presentFamily.has_value(); }
		};
	public:
		Device(const VkInstance& instance, bool enableValidationLayers, const std::vector<const char*>& validationLayers, const VkSurfaceKHR& surface);
		~Device();
		Device(const Device&) = delete;
		Device& operator=(const Device&) = delete;
		void createPhysicalDevice(const VkInstance& instance);
		void createLogicalDevice(bool enableValidationLayers, const std::vector<const char*>& validationLayers);
	private:
		void pickPhysicalDevice(const VkInstance& instance);
		bool isDeviceSuitable(VkPhysicalDevice device);
		QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
	private:
		VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
		VkPhysicalDeviceProperties properties;
		VkDevice device;
		VkQueue graphicsQueue;
		VkQueue presentQueue;
		const VkSurfaceKHR& pSurface;
	};
}