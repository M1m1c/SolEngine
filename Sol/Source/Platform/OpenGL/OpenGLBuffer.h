//#pragma once
//#include "Sol/Renderer/Buffer.h"
//
//namespace Sol {
//	class OpenGlVertexBuffer : public VertexBuffer
//	{
//	public:
//		OpenGlVertexBuffer(float* verts, uint32_t size);
//		virtual ~OpenGlVertexBuffer();
//		virtual void Bind() const;
//		virtual void UnBind() const;
//	private:
//		uint32_t m_RendererID;
//	};
//
//	class OpenGlIndexBuffer : public IndexBuffer
//	{
//	public:
//		OpenGlIndexBuffer(uint32_t* indices, uint32_t size);
//		
//		virtual ~OpenGlIndexBuffer();
//		virtual void Bind() const;
//		virtual void UnBind() const;
//
//		virtual uint32_t OpenGlIndexBuffer::GetCount() const override { return m_Count; }
//
//	private:
//		uint32_t m_RendererID;
//
//		uint32_t m_Count;
//	};
//}