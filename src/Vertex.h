#pragma once

#include <vulkan/vulkan.h>
#include <glm/glm.hpp>
#include <vector>

namespace FVulkanEngine
{
	class VertexLayout
	{
	public:
		enum ElementType
		{
			Position2D,
			Position3D,
			Texture2D,
			Normal,
			Float3Color,
			Float4Color,
		};
		template<ElementType> struct Map;
		template<> struct Map<Position2D>
		{
			using SysType = glm::vec2;
			static constexpr VkFormat format = VK_FORMAT_R32G32_SFLOAT;
			static constexpr const char* semantic = "Position";
		};
		template<> struct Map<Position3D>
		{
			using SysType = glm::vec3;
			static constexpr VkFormat format = VK_FORMAT_R32G32B32_SFLOAT;
			static constexpr const char* semantic = "Position";
		};
		template<> struct Map<Texture2D>
		{
			using SysType = glm::vec2;
			static constexpr VkFormat format = VK_FORMAT_R32G32_SFLOAT;
			static constexpr const char* semantic = "Texcoord";
		};
		template<> struct Map<Normal>
		{
			using SysType = glm::vec3;
			static constexpr VkFormat format = VK_FORMAT_R32G32B32_SFLOAT;
			static constexpr const char* semantic = "Normal";
		};
		template<> struct Map<Float3Color>
		{
			using SysType = glm::vec3;
			static constexpr VkFormat format = VK_FORMAT_R32G32B32_SFLOAT;
			static constexpr const char* semantic = "Color";
		};
		template<> struct Map<Float4Color>
		{
			using SysType = glm::vec4;
			static constexpr VkFormat format = VK_FORMAT_R32G32B32A32_SFLOAT;
			static constexpr const char* semantic = "Color";
		};
		class Element
		{
		public:
			Element(ElementType type, size_t offset) : type{ type }, offset{ offset }{}
			size_t getOffsetAfter() const { return offset + typeSize(); }
			size_t getOffset() const { return offset; }
			size_t typeSize() const { return getSize(type); }
			static constexpr size_t getSize(ElementType type)
			{
				switch (type)
				{
				case FVulkanEngine::VertexLayout::Position2D:
					return sizeof(Map<Position2D>::SysType);
				case FVulkanEngine::VertexLayout::Position3D:
					return sizeof(Map<Position3D>::SysType);
				case FVulkanEngine::VertexLayout::Texture2D:
					return sizeof(Map<Texture2D>::SysType);
				case FVulkanEngine::VertexLayout::Normal:
					return sizeof(Map<Normal>::SysType);
				case FVulkanEngine::VertexLayout::Float3Color:
					return sizeof(Map<Float3Color>::SysType);
				case FVulkanEngine::VertexLayout::Float4Color:
					return sizeof(Map<Float4Color>::SysType);
				}
				assert("invalid element type" && false);
				return 0u;
			}
			ElementType getType() const { return type; }
		private:
			ElementType type;
			size_t offset;
		};
	public:
		template<ElementType Type>
		const Element& resolve() const
		{
			for (auto& elm : elements)
			{
				if (elm.getType() == Type)
				{
					return elm;
				}
			}
			assert("could not resolve element type" && false);
			return elements.front();
		}
		const Element& resolveByIndex(size_t i) const { return elements[i]; }
		VertexLayout& append(ElementType type)
		{
			elements.emplace_back(type, getSize());
			return *this;
		}
		size_t getSize() const { return elements.empty() ? 0u : elements.back().getOffsetAfter(); }
		size_t getElementCount() const { return elements.size(); }
	private:
		std::vector<Element> elements;
	};
	class Vertex
	{
		friend class VertexBuffer;
	public:
		template<VertexLayout::ElementType Type>
		auto& getAttribute()
		{
			auto pAttribute = pData + layout.Resolve<Type>().GetOffset();
			return *reinterpret_cast<typename VertexLayout::Map<Type>::SysType*>(pAttribute);
		}
		template<typename T>
		void setAttributeByIndex(size_t i, T&& val)
		{
			const auto& element = layout.resolveByIndex(i);
			auto pAttribute = pData + element.getOffset();
			switch (element.getType())
			{
			case VertexLayout::Position2D:
				setAttribute<VertexLayout::Position2D>(pAttribute, std::forward<T>(val));
				break;
			case VertexLayout::Position3D:
				setAttribute<VertexLayout::Position3D>(pAttribute, std::forward<T>(val));
				break;
			case VertexLayout::Texture2D:
				setAttribute<VertexLayout::Texture2D>(pAttribute, std::forward<T>(val));
				break;
			case VertexLayout::Normal:
				setAttribute<VertexLayout::Normal>(pAttribute, std::forward<T>(val));
				break;
			case VertexLayout::Float3Color:
				setAttribute<VertexLayout::Float3Color>(pAttribute, std::forward<T>(val));
				break;
			case VertexLayout::Float4Color:
				setAttribute<VertexLayout::Float4Color>(pAttribute, std::forward<T>(val));
				break;
			default:
				assert("bad element type" && false);
			}
		}
	private:
		Vertex(char* pData, const VertexLayout& layout) : pData{ pData }, layout{ layout } { assert(pData != nullptr); }
		template<typename First, typename... Rest>
		void setAttributeByIndex(size_t i, First&& first, Rest&&... rest)
		{
			setAttributeByIndex(i, std::forward(first));
			setAttributeByIndex(i + 1, std::forward(rest)...);
		}
		template<VertexLayout::ElementType DestLayoutType, typename SrcType>
		void setAttribute(char* pAttribute, SrcType&& val)
		{
			using Dest = typename VertexLayout::Map<DestLayoutType>::SysType;
			if constexpr (std::is_assignable<Dest, SrcType>::value)
			{
				*reinterpret_cast<Dest*>(pAttribute) = val;
			}
			else
			{
				assert("parameter attribute type mismatch" && false);
			}
		}
	private:
		char* pData = nullptr;
		const VertexLayout& layout;
	};
	class ConstVertex
	{
	public:
		ConstVertex(const Vertex& v) : vertex(v) {}
		template<VertexLayout::ElementType Type>
		const auto& getAttribute() const
		{
			return const_cast<Vertex&>(vertex).getAttribute<Type>();
		}
	private:
		Vertex vertex;
	};
	class VertexBuffer
	{
	public:
		VertexBuffer(VertexLayout layout) : layout(std::move(layout)) {}
		const char* getData() const { return buffer.data(); }
		const VertexLayout& getLayout() const { return layout; }
		size_t getSize() const { return buffer.size() / layout.getSize(); }
		size_t sizeBytes() const { return buffer.size(); }
		template<typename... Params>
		void emplaceBack(Params&&... params)
		{
			assert(sizeof...(params) == layout.getElementCount() && "param count does not match number of vertex elements");
			buffer.resize(buffer.size() + layout.getSize());
			back().SetAttributeByIndex(0u, std::forward<Params>(params)...);
		}
		Vertex back()
		{
			assert(buffer.size() != 0u);
			return Vertex{ buffer.data() + buffer.size() - layout.getSize(), layout };
		}
		Vertex front()
		{
			assert(buffer.size() != 0u);
			return Vertex{ buffer.data(), layout };
		}
		Vertex operator[](size_t i)
		{
			assert(i < getSize());
			return Vertex{ buffer.data() + layout.getSize() * i, layout };
		}
		ConstVertex back() const
		{
			return const_cast<VertexBuffer*>(this)->back();
		}
		ConstVertex front() const
		{
			return const_cast<VertexBuffer*>(this)->front();
		}
		ConstVertex operator[](size_t i) const
		{
			return const_cast<VertexBuffer&>(*this)[i];
		}
	private:
		std::vector<char> buffer;
		VertexLayout layout;
	};
}