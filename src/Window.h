#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <string>

namespace FVulkanEngine
{
	class Window
	{
	public:
		Window(uint32_t width, uint32_t height, std::string name);
		~Window();
		Window(const Window&) = delete;
		Window& operator=(const Window&) = delete;
	public:
		bool shouldClose();
		void createSurface(const VkInstance& instance);
		void destructSurface(const VkInstance& instance);
		const VkSurfaceKHR& getSurface();
		GLFWwindow* getGLFWwindowPointer() const;
		const VkSurfaceKHR& getWindowSurface() const;
	private:
		uint32_t width = 800;
		uint32_t height = 600;
		std::string name;
		GLFWwindow* window;
		VkSurfaceKHR surface;
	};
}