#pragma once

#include "Device.h"
#include "SwapChain.h"
#include <string>

namespace FVulkanEngine
{
	class Pipeline
	{
	public:
		struct PipelineConfigInfo
		{
			PipelineConfigInfo() = default;
			PipelineConfigInfo(const PipelineConfigInfo&) = delete;
			PipelineConfigInfo& operator=(const PipelineConfigInfo&) = delete;
			VkPipelineViewportStateCreateInfo viewportInfo{};
			VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo{};
			VkPipelineRasterizationStateCreateInfo rasterizationInfo{};
			VkPipelineMultisampleStateCreateInfo multisampleInfo{};
			VkPipelineColorBlendAttachmentState colorBlendAttachment{};
			VkPipelineColorBlendStateCreateInfo colorBlendInfo{};
			VkPipelineDepthStencilStateCreateInfo depthStencilInfo{};
			std::vector<VkDynamicState> dynamicStateEnables;
			VkPipelineDynamicStateCreateInfo dynamicStateInfo{};
			VkPipelineLayout pipelineLayout = nullptr;
			VkRenderPass renderPass = nullptr;
			uint32_t subpass = 0;

			//remove later
			VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
		};
	public:
		Pipeline(Device& device, SwapChain& swapChain,const PipelineConfigInfo& configInfo, const std::string& vertFilepath, const std::string& fragFilepath);
		~Pipeline();
		Pipeline(const Pipeline&) = delete;
		Pipeline& operator=(const Pipeline&) = delete;
	public:
		static void defaultPipelineConfigInfo(PipelineConfigInfo& configInfo);
	private:
		void createPipeline(const PipelineConfigInfo& configInfo, const std::string& vertFilepath, const std::string& fragFilepath);
		void createRenderPass();
	private:
		Device& pDevice;
		SwapChain& pSwapChain;
		VkRenderPass renderPass;
		VkPipelineLayout pipelineLayout;
	};
}