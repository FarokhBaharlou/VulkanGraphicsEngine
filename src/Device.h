#pragma once
#include <vulkan/vulkan.h>
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
		Device();
		~Device();
		Device(const Device&) = delete;
		Device& operator=(const Device&) = delete;
		void create(const VkInstance& instance);
	private:
		void pickPhysicalDevice(const VkInstance& instance);
		bool isDeviceSuitable(VkPhysicalDevice device);
		QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
	private:
		VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
		VkPhysicalDeviceProperties properties;
	};
}