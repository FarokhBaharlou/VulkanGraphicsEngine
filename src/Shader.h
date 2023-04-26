#pragma once

#include "Device.h"
#include <vulkan/vulkan.h>
#include <vector>
#include <string>

namespace FVulkanEngine
{
	class Shader
	{
	public:
		Shader(Device& device, const std::string& filename);
		~Shader();
		Shader(const Shader&) = delete;
		Shader& operator=(const Shader&) = delete;
	public:
		const VkShaderModule& getShaderModule();
	private:
		std::vector<char> readFile(const std::string& filename);
	private:
		Device& pDevice;
		VkShaderModule shaderModule;
	};
}