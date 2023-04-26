#include "Pipeline.h"
#include "Shader.h"

namespace FVulkanEngine
{
	Pipeline::Pipeline(Device& device, const std::string& vertFilepath, const std::string& fragFilepath) : pDevice{ device }
	{
		createPipeline(vertFilepath, fragFilepath);
	}
	Pipeline::~Pipeline()
	{

	}
	void Pipeline::createPipeline(const std::string& vertFilepath, const std::string& fragFilepath)
	{
		Shader vertexShader{ pDevice, vertFilepath };
		Shader fragmentShader{ pDevice, fragFilepath };

		VkPipelineShaderStageCreateInfo vertexShaderStageInfo{};
		vertexShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		vertexShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
		vertexShaderStageInfo.module = vertexShader.getShaderModule();
		vertexShaderStageInfo.pName = "main";
		VkPipelineShaderStageCreateInfo fragmentShaderStageInfo{};
		fragmentShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		fragmentShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		fragmentShaderStageInfo.module = fragmentShader.getShaderModule();
		fragmentShaderStageInfo.pName = "main";
		VkPipelineShaderStageCreateInfo shaderStages[] = { vertexShaderStageInfo, fragmentShaderStageInfo };
	}
}