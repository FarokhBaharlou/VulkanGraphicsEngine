#pragma once

#include "Device.h"
#include "Window.h"
#include <vector>

namespace FVulkanEngine
{
	class SwapChain
	{
	public:
		SwapChain(const Device& device, const Window& window);
		~SwapChain();
		SwapChain(const SwapChain&) = default;
		SwapChain& operator=(const SwapChain&) = default;
	private:
		void createSwapChain();
		VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
		VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
		VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
	private:
		const Device& pDevice;
		const Window& pWindow;
		VkSwapchainKHR swapChain;
		std::vector<VkImage> swapChainImages;
		VkFormat swapChainImageFormat;
		VkExtent2D swapChainExtent;
	};
}