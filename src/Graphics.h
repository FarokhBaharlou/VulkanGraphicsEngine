#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>

namespace FVulkanEngine
{
	class Graphics
	{
	public:
		Graphics();
		~Graphics();
		Graphics(const Graphics&) = delete;
		Graphics& operator=(const Graphics&) = delete;
	private:
		std::vector<const char*> getRequiredExtensions();
		void hasGlfwRequiredExtensions();
	private:
		VkInstance instance;
	};
}