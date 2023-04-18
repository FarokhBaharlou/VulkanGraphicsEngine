#include "Graphics.h"
#include <iostream>
#include <stdexcept>
#include <unordered_set>
#include <string>

namespace FVulkanEngine
{
	Graphics::Graphics()
	{
		VkApplicationInfo appInfo{};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = "FVulkanEngine";
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName = "No Engine";
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion = VK_API_VERSION_1_0;

		VkInstanceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;
		auto extensions = getRequiredExtensions();
		createInfo.enabledExtensionCount = static_cast<int32_t>(extensions.size());
		createInfo.ppEnabledExtensionNames = extensions.data();
		createInfo.enabledLayerCount = 0;

		if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create instance.");
		}
		hasGlfwRequiredExtensions();
	}
	Graphics::~Graphics()
	{
		vkDestroyInstance(instance, nullptr);
	}
	std::vector<const char*> Graphics::getRequiredExtensions()
	{
		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions;
		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
		std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
		return extensions;
	}
	void Graphics::hasGlfwRequiredExtensions()
	{
		uint32_t extensionCount = 0;
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
		std::vector<VkExtensionProperties> extensions(extensionCount);
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

		std::unordered_set<std::string> avalibleExtensions;
		std::cout << "avalible extensions:" << std::endl;
		for (const auto& extension : extensions)
		{
			avalibleExtensions.insert(extension.extensionName);
			std::cout << '\t' << extension.extensionName << std::endl;
		}

		std::cout << "required extensions:" << std::endl;
		auto requiredExtensions = getRequiredExtensions();
		for (const auto& extension : requiredExtensions)
		{
			std::cout << '\t' << extension << std::endl;
			if (avalibleExtensions.find(extension) == avalibleExtensions.end())
			{
				throw std::runtime_error("missing required extension");
			}
		}
	}
}