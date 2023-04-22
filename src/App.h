#pragma once
#include "Window.h"
#include "Graphics.h"

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
		Graphics gfx{};
	};
}