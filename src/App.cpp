#include "App.h"

namespace FVulkanEngine
{
	App::App() {}
	App::~App() {}
	void App::run()
	{
		while (!gfx.getWindow()->shouldClose())
		{
			glfwPollEvents();
		}
	}
}