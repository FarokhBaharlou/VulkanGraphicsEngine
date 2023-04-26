#pragma once

#include "Device.h"
#include <string>

namespace FVulkanEngine
{
	class Pipeline
	{
	public:
		Pipeline(Device& device, const std::string& vertFilepath, const std::string& fragFilepath);
		~Pipeline();
		Pipeline(const Pipeline&) = delete;
		Pipeline& operator=(const Pipeline&) = delete;
	private:
		void createPipeline(const std::string& vertFilepath, const std::string& fragFilepath);
	private:
		Device& pDevice;
	};
}