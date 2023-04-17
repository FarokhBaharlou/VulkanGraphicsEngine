#include "Window.h"

namespace FVulkanEngine
{
	Window::Window(uint32_t width, uint32_t height, std::string name) : width{width}, height{height}, name{name}
	{
		glfwInit();

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		window = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);
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
}