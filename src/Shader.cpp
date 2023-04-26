#include "Shader.h"
#include <fstream>
#include <iostream>

namespace FVulkanEngine
{
	Shader::Shader(Device& device, const std::string& filename) : pDevice{ device }
	{
		const auto shaderCode = readFile(filename);

		VkShaderModuleCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = shaderCode.size();
		createInfo.pCode = reinterpret_cast<const uint32_t*>(shaderCode.data());
		if (vkCreateShaderModule(pDevice.getDevice(), &createInfo, nullptr, &shaderModule) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create shader module");
		}
	}
	Shader::~Shader()
	{
		vkDestroyShaderModule(pDevice.getDevice(), shaderModule, nullptr);
	}
	std::vector<char> Shader::readFile(const std::string& filename)
	{
		std::ifstream file(filename, std::ios::ate | std::ios::binary);
		if (!file.is_open())
		{
			throw std::runtime_error("failed to open file");
		}
		size_t fileSize = (size_t)file.tellg();
		std::vector<char> buffer(fileSize);
		file.seekg(0);
		file.read(buffer.data(), fileSize);
		file.close();
		return buffer;
	}
	const VkShaderModule& Shader::getShaderModule()
	{
		return shaderModule;
	}
}