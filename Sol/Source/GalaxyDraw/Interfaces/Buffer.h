#pragma once
#include <Sol/Core/Core.h>

namespace GalaxyDraw
{
	enum class ShaderDataType {
		none = 0,
		Float,
		Float2,
		Float3,
		Float4,
		Mat3,
		Mat4,
		Int,
		Int2,
		Int3,
		Int4,
		Bool

	};

	static uint32_t ShaderDataTypeSize(ShaderDataType type)
	{
		switch (type)
		{
		case GalaxyDraw::ShaderDataType::Float:		return 4;
		case GalaxyDraw::ShaderDataType::Float2:	return 4 * 2;
		case GalaxyDraw::ShaderDataType::Float3:	return 4 * 3;
		case GalaxyDraw::ShaderDataType::Float4:	return 4 * 4;
		case GalaxyDraw::ShaderDataType::Mat3:		return 4 * 3 * 3;
		case GalaxyDraw::ShaderDataType::Mat4:		return 4 * 4 * 4;
		case GalaxyDraw::ShaderDataType::Int:		return 4;
		case GalaxyDraw::ShaderDataType::Int2:		return 4 * 2;
		case GalaxyDraw::ShaderDataType::Int3:		return 4 * 3;
		case GalaxyDraw::ShaderDataType::Int4:		return 4 * 4;
		case GalaxyDraw::ShaderDataType::Bool:		return 1;
		}
		SOL_CORE_ASSERT(false, "Unkown ShaderDataType!")
			return 0;
	}

	struct BufferElement
	{
		std::string Name;
		ShaderDataType Type;
		uint32_t Size;
		uint32_t Offset;
		bool Normalized;

		BufferElement() {}
		BufferElement(ShaderDataType type, const std::string& name, bool normalized = false) :
			Type(type),
			Name(name),
			Size(ShaderDataTypeSize(type)),
			Offset(0),
			Normalized(normalized)
		{}

		uint32_t GetComponentCount() const
		{
			switch (Type)
			{
			case GalaxyDraw::ShaderDataType::Float:		return 1;
			case GalaxyDraw::ShaderDataType::Float2:	return 2;
			case GalaxyDraw::ShaderDataType::Float3:	return 3;
			case GalaxyDraw::ShaderDataType::Float4:	return 4;
			case GalaxyDraw::ShaderDataType::Mat3:		return 3 * 3;
			case GalaxyDraw::ShaderDataType::Mat4:		return 4 * 4;
			case GalaxyDraw::ShaderDataType::Int:		return 1;
			case GalaxyDraw::ShaderDataType::Int2:		return 2;
			case GalaxyDraw::ShaderDataType::Int3:		return 3;
			case GalaxyDraw::ShaderDataType::Int4:		return 4;
			case GalaxyDraw::ShaderDataType::Bool:		return 1;
			}
			SOL_CORE_ASSERT(false, "Unkown ShaderDataType!")
				return 0;
		}
	};

	//This is used for determining the paramters sent to glVertexAttribPointer() in the instance buffer
	struct VertexAttributeSpecs
	{
			uint8_t TypeSize;
			//GLenum type,
			//GLboolean normalized,
			//GLsizei stride,
			size_t PointerOffset;

			VertexAttributeSpecs(){}
			VertexAttributeSpecs(uint8_t typeSize,size_t pointerOffset) :
				TypeSize(typeSize), PointerOffset(pointerOffset) {}
	};


	class BufferLayout
	{
	public:
		BufferLayout() {}
		BufferLayout(const std::initializer_list<BufferElement>& elements) :m_Elements(elements)
		{
			CalculateOffsetAndStride();
		}

		inline const std::vector<BufferElement>& GetElements() const { return m_Elements; }
		inline uint32_t GetStride() const { return m_Stride; }

		std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
		std::vector<BufferElement>::iterator end() { return m_Elements.end(); }

		std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
		std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }
	private:
		void CalculateOffsetAndStride()
		{
			uint32_t offset = 0;
			m_Stride = 0;
			for (auto& element : m_Elements)
			{
				element.Offset = offset;
				offset += element.Size;
				m_Stride += element.Size;
			}
		}

		std::vector<BufferElement> m_Elements;
		uint32_t m_Stride = 0;
	};

	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() {};

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void SetData(const void* data,uint32_t size) = 0;
		virtual void SetLayout(const BufferLayout& layout) = 0;
		virtual const BufferLayout& GetLayout() const = 0;


		static std::shared_ptr <VertexBuffer> Create(uint32_t size);
		static std::shared_ptr <VertexBuffer> Create(float* verts, uint32_t size);
	};

	//currently only supports 32 bit index buffers
	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() {};

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual uint32_t GetCount()const = 0;

		static std::shared_ptr <IndexBuffer> Create(uint32_t* indices, uint32_t count);
		static std::shared_ptr <IndexBuffer> Create(const uint32_t* indices, uint32_t count);
	};

	class InstanceBuffer
	{
	public:
		virtual ~InstanceBuffer() {};

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void SetData(const void* data, uint32_t size) = 0;
		//virtual void SetVertexAttributes(uint32_t numAttriutes) = 0;

		static std::shared_ptr <InstanceBuffer> Create(uint32_t size, uint32_t stride, std::vector<VertexAttributeSpecs> specs, uint32_t layoutOffset);
		//static std::shared_ptr <InstanceBuffer> Create(const uint32_t* indices, uint32_t count);
	};
}

