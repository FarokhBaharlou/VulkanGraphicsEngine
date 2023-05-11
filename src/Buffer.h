#pragma once
#include "Device.h"
#include "Vertex.h"

namespace FVulkanEngine
{
	class Buffer
	{
	public:
		Buffer(Device& device, VertexBuffer& vb);
		~Buffer();
		VkBuffer getVkBuffer();
		const VertexBuffer& getBuffer();
	private:
		void createVertexBuffer();
		uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
	private:
		Device& device;
		VertexBuffer vb;
		VkBuffer vertexBuffer;
		VkDeviceMemory vertexBufferMemory;
	};
}