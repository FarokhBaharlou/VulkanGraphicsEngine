#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "Device.h"
#include "Window.h"
#include <vector>
#include <memory>

namespace FVulkanEngine
{
	class Graphics
	{
	public:
		Graphics();
		~Graphics();
		Graphics(const Graphics&) = delete;
		Graphics& operator=(const Graphics&) = delete;
	public:
		const std::unique_ptr<Window>& getWindow();
	private:
		std::vector<const char*> getRequiredExtensions();
		void hasGlfwRequiredExtensions();
		bool checkValidationLayerSupport();
		void createInstance();
		static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);
		void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
		void setupDebugMessenger();
	private:
	#ifdef NDEBUG
		const bool enableValidationLayers = false;
	#else
		const bool enableValidationLayers = true;
	#endif
		VkInstance instance;
		VkDebugUtilsMessengerEXT debugMessenger;
		const std::vector<const char*> validationLayers = { "VK_LAYER_KHRONOS_validation" };
		std::unique_ptr<Device> device;
		std::unique_ptr<Window> window;
	};
}