#pragma once
#include "Window.h"

namespace FVulkanEngine
{
	class App
	{
	public:
		App();
		~App();
		App(const App&) = delete;
		App& operator=(const App&) = delete;
	public:
		void run();
	private:
		Window window{800, 600, "FVulkanEngine"};
	};
}