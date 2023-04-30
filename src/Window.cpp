#include "Window.h"
#include <iostream>

namespace FVulkanEngine
{
	Window::Window(uint32_t width, uint32_t height, std::string name) : width{width}, height{height}, name{name}
	{
		glfwInit();

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		//glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		window = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);
		glfwSetWindowUserPointer(window, this);
		glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
	}
	Window::~Window()
	{
		glfwDestroyWindow(window);
		glfwTerminate();
	}
	bool Window::shouldClose()
	{
		return glfwWindowShouldClose(window);
	}
	void Window::createSurface(const VkInstance& instance)
	{
		if (glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create window Surface");
		}
	}
	void Window::destructSurface(const VkInstance& instance)
	{
		vkDestroySurfaceKHR(instance, surface, nullptr);
	}
	const VkSurfaceKHR& Window::getSurface()
	{
		return surface;
	}
	GLFWwindow* Window::getGLFWwindowPointer() const
	{
		return window;
	}
	const VkSurfaceKHR& Window::getWindowSurface() const
	{
		return surface;
	}
	void Window::framebufferResizeCallback(GLFWwindow* window, int width, int height)
	{
		auto wnd = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
		wnd->framebufferResized = true;
		wnd->width = width;
		wnd->height = height;
	}
}