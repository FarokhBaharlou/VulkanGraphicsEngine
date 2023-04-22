#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <optional>

namespace FVulkanEngine
{
	class Device
	{
	private:
		struct QueueFamilyIndices
		{
			std::optional<int32_t> graphicsFamily;
			bool isComplete() { return graphicsFamily.has_value(); }
		};
	public:
		Device(const VkInstance& instance, bool enableValidationLayers, const std::vector<const char*>& validationLayers);
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
	};
}