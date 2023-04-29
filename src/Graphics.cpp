#include "Graphics.h"
#include <iostream>
#include <stdexcept>
#include <unordered_set>
#include <string>

namespace FVulkanEngine
{
	VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger)
	{
		auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
		if (func != nullptr)
		{
			return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
		}
		else
		{
			return VK_ERROR_EXTENSION_NOT_PRESENT;
		}
	}
	void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator)
	{
		auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
		if (func != nullptr)
		{
			func(instance, debugMessenger, pAllocator);
		}
	}
	VKAPI_ATTR VkBool32 VKAPI_CALL Graphics::debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
	{
		std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;
		return VK_FALSE;
	}
	Graphics::Graphics()
	{
		window = std::make_unique<Window>(800, 600, "FVulkanEngine");
		createInstance();
		window->createSurface(instance);
		setupDebugMessenger();
		device = std::make_unique<Device>(instance, enableValidationLayers, validationLayers, window->getSurface());
		swapChain = std::make_unique<SwapChain>(*device, *window);
		Pipeline::PipelineConfigInfo pipelineConfigInfo{};
		Pipeline::defaultPipelineConfigInfo(pipelineConfigInfo);
		pipeline = std::make_unique<Pipeline>(*device, *swapChain, pipelineConfigInfo, "shaders/simple_vertex_shader.vert.spv", "shaders/simple_fragment_shader.frag.spv");
		swapChain->createFramebuffers(pipeline->getRenderPass());
		createSyncObjects();
	}
	Graphics::~Graphics()
	{
		vkDestroySemaphore(device->getDevice(), renderFinishedSemaphore, nullptr);
		vkDestroySemaphore(device->getDevice(), imageAvailableSemaphore, nullptr);
		vkDestroyFence(device->getDevice(), inFlightFence, nullptr);
		pipeline.reset();
		swapChain.reset();
		device.reset();
		if (enableValidationLayers)
		{
			DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
		}
		window->destructSurface(instance);
		vkDestroyInstance(instance, nullptr);
	}
	std::vector<const char*> Graphics::getRequiredExtensions()
	{
		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions;
		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
		std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

		if (enableValidationLayers)
		{
			extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		}
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
	bool Graphics::checkValidationLayerSupport()
	{
		uint32_t layerCount = 0;
		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
		std::vector<VkLayerProperties> avalibleLayers(layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount, avalibleLayers.data());

		for (const char* layerName : validationLayers)
		{
			bool layerFound = false;
			for (const auto& layerProperties : avalibleLayers)
			{
				if (strcmp(layerName, layerProperties.layerName) == 0)
				{
					layerFound = true;
					break;
				}
			}
			if (!layerFound)
			{
				return false;
			}
		}
		return true;
	}
	void Graphics::createInstance()
	{
		if (enableValidationLayers && !checkValidationLayerSupport())
		{
			throw std::runtime_error("validation layers requested, but not avalible.");
		}

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

		VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
		if (enableValidationLayers)
		{
			createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
			createInfo.ppEnabledLayerNames = validationLayers.data();
			populateDebugMessengerCreateInfo(debugCreateInfo);
			createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
		}
		else
		{
			createInfo.enabledLayerCount = 0;
			createInfo.pNext = nullptr;
		}

		if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create instance.");
		}
		hasGlfwRequiredExtensions();
	}
	void Graphics::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo)
	{
		createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		createInfo.pfnUserCallback = debugCallback;
		createInfo.pUserData = nullptr;
	}
	void Graphics::setupDebugMessenger()
	{
		if (!enableValidationLayers) return;
		VkDebugUtilsMessengerCreateInfoEXT createInfo{};
		populateDebugMessengerCreateInfo(createInfo);
		if (CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to set up debug messenger.");
		}
	}
	const std::unique_ptr<Window>& Graphics::getWindow()
	{
		return window;
	}
	void Graphics::createSyncObjects()
	{
		VkSemaphoreCreateInfo semaphoreInfo{};
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
		VkFenceCreateInfo fenceInfo{};
		fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
		if (vkCreateSemaphore(device->getDevice(), &semaphoreInfo, nullptr, &imageAvailableSemaphore) != VK_SUCCESS ||
			vkCreateSemaphore(device->getDevice(), &semaphoreInfo, nullptr, &renderFinishedSemaphore) != VK_SUCCESS ||
			vkCreateFence(device->getDevice(), &fenceInfo, nullptr, &inFlightFence) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create sync objects");
		}
	}
	void Graphics::drawFrame()
	{
		vkWaitForFences(device->getDevice(), 1, &inFlightFence, VK_TRUE, UINT64_MAX);
		vkResetFences(device->getDevice(), 1, &inFlightFence);

		uint32_t imageIndex;
		vkAcquireNextImageKHR(device->getDevice(), swapChain->getSwapChain(), UINT64_MAX, imageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex);

		vkResetCommandBuffer(device->getCommandBuffer(), 0);
		pipeline->recordCommandBuffer(imageIndex);

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		VkSemaphore waitSemaphores[] = { imageAvailableSemaphore };
		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = waitSemaphores;
		submitInfo.pWaitDstStageMask = waitStages;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &device->getCommandBuffer();
		VkSemaphore signalSemaphores[] = { renderFinishedSemaphore };
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = signalSemaphores;
		if (vkQueueSubmit(device->getGraphicsQueue(), 1, &submitInfo, inFlightFence) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to submit draw command buffer");
		}

		VkPresentInfoKHR presentInfo{};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = signalSemaphores;
		VkSwapchainKHR swapChains[] = { swapChain->getSwapChain() };
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = swapChains;
		presentInfo.pImageIndices = &imageIndex;
		presentInfo.pResults = nullptr;
		vkQueuePresentKHR(device->getPresentQueue(), &presentInfo);
	}
	const VkDevice& Graphics::getDevice() const
	{
		return device->getDevice();
	}
}