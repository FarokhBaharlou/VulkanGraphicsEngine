#include "Buffer.h"

namespace FVulkanEngine
{
	Buffer::Buffer(Device& device, VertexBuffer& vb) : device{ device }, vb{ std::move(vb) }
	{
		createVertexBuffer();
	}
	Buffer::~Buffer()
	{
		vkDestroyBuffer(device.getDevice(), vertexBuffer, nullptr);
		vkFreeMemory(device.getDevice(), vertexBufferMemory, nullptr);
	}
	void Buffer::createVertexBuffer()
	{
		VkBufferCreateInfo bufferInfo{};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = vb.getBuffer().size();
		bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		if (vkCreateBuffer(device.getDevice(), &bufferInfo, nullptr, &vertexBuffer) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create vertex buffer");
		}
		VkMemoryRequirements memRequirements;
		vkGetBufferMemoryRequirements(device.getDevice(), vertexBuffer, &memRequirements);
		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
		if (vkAllocateMemory(device.getDevice(), &allocInfo, nullptr, &vertexBufferMemory) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to allocate vertex buffer memory");
		}
		vkBindBufferMemory(device.getDevice(), vertexBuffer, vertexBufferMemory, 0);

		void* data;
		vkMapMemory(device.getDevice(), vertexBufferMemory, 0, bufferInfo.size, 0, &data);
		memcpy(data, vb.getBuffer().data(), static_cast<size_t>(bufferInfo.size));
		vkUnmapMemory(device.getDevice(), vertexBufferMemory);
	}
	uint32_t Buffer::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
	{
		VkPhysicalDeviceMemoryProperties memProperties;
		vkGetPhysicalDeviceMemoryProperties(device.getPhysicalDevcie(), &memProperties);
		for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
		{
			if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
			{
				return i;
			}
		}
		throw std::runtime_error("failed to find suitable memory type");
		return 0;
	}
	VkBuffer Buffer::getVkBuffer() { return vertexBuffer; }
	const VertexBuffer& Buffer::getBuffer() { return vb; }
}