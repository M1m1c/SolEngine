#pragma once

#define GDraw_VBO GalaxyDraw::VertexBuffer
#define GDraw_EBO GalaxyDraw::IndexBuffer

namespace GalaxyDraw 
{
	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() {};

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		static VertexBuffer* Create(float* verts,uint32_t size);
	};

	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() {};

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual uint32_t GetCount()const = 0;

		static IndexBuffer* Create(uint32_t* indices, uint32_t count);
	};
}