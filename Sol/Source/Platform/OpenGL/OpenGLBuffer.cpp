#include "solpch.h"
//#include "OpenGLBuffer.h"
//
//#include <glad/glad.h>
//
//namespace Sol 
//{
//	//////////////////////////////////////////////////////////////////////////////
//	//VERTEX BUFFER
//	OpenGlVertexBuffer::OpenGlVertexBuffer(float* verts, uint32_t size)
//	{
//		glCreateBuffers(1, &m_RendererID);
//		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
//		glBufferData(GL_ARRAY_BUFFER, size, verts, GL_STATIC_DRAW);
//	}
//
//	OpenGlVertexBuffer::~OpenGlVertexBuffer()
//	{
//		glDeleteBuffers(1, &m_RendererID);
//	}
//
//	void OpenGlVertexBuffer::Bind() const
//	{
//		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
//	}
//
//	void OpenGlVertexBuffer::UnBind() const
//	{
//		glBindBuffer(GL_ARRAY_BUFFER, 0);
//	}
//
//	//////////////////////////////////////////////////////////////////////////////
//	//INDEX BUFFER
//	OpenGlIndexBuffer::OpenGlIndexBuffer(uint32_t* indices, uint32_t count) : m_Count(count)
//	{
//		glCreateBuffers(1, &m_RendererID);
//		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
//		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
//		
//	}
//
//	OpenGlIndexBuffer::~OpenGlIndexBuffer()
//	{
//		glDeleteBuffers(1, &m_RendererID);
//	}
//
//	void OpenGlIndexBuffer::Bind() const
//	{
//		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
//	}
//
//	void OpenGlIndexBuffer::UnBind() const
//	{
//		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
//	}
//
//
//}