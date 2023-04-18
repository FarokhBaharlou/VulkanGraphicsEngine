#include "Device.h"
#include <stdexcept>
#include <vector>
#include <iostream>

namespace FVulkanEngine
{
	Device::Device() {}
	Device::~Device() {}
	void Device::create(const VkInstance& instance)
	{
		pickPhysicalDevice(instance);
	}
	void Device::pickPhysicalDevice(const VkInstance& instance)
	{
		uint32_t deviceCount = 0;
		vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
		if (deviceCount == 0)
		{
			throw std::runtime_error("failed to find GPUs with vulkan support.");
		}
		std::cout << "Device count: " << deviceCount << std::endl;
		std::vector<VkPhysicalDevice> devices(deviceCount);
		vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());
		for (const auto& device : devices)
		{
			if (isDeviceSuitable(device))
			{
				physicalDevice = device;
				break;
			}
		}
		if (physicalDevice == VK_NULL_HANDLE)
		{
			throw std::runtime_error("failed to find a suitable GPU.");
		}
		vkGetPhysicalDeviceProperties(physicalDevice, &properties);
		std::cout << "physical device: " << properties.deviceName << std::endl;
	}
	bool Device::isDeviceSuitable(VkPhysicalDevice device)
	{
		QueueFamilyIndices indices = findQueueFamilies(device);
		return indices.isComplete();
	}
	Device::QueueFamilyIndices Device::findQueueFamilies(VkPhysicalDevice device)
	{
		QueueFamilyIndices indices;

		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

		int i = 0;
		for (const auto& queueFamily : queueFamilies)
		{
			if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
			{
				indices.graphicsFamily = i;
				break;
			}
			i++;
		}

		return indices;
	}
}