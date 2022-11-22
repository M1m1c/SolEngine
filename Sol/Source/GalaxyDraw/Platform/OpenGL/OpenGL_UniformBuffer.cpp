#include "solpch.h"
#include "OpenGL_UniformBuffer.h"

#include <glad/glad.h>

namespace GalaxyDraw {

	OpenGL_UniformBuffer::OpenGL_UniformBuffer(uint32_t size, uint32_t binding)
	{
		glCreateBuffers(1, &m_RendererID);
		glNamedBufferData(m_RendererID, size, nullptr, GL_DYNAMIC_DRAW); // TODO: investigate usage hint
		glBindBufferBase(GL_UNIFORM_BUFFER, binding, m_RendererID);
	}

	OpenGL_UniformBuffer::~OpenGL_UniformBuffer()
	{
		glDeleteBuffers(1, &m_RendererID);
	}


	void OpenGL_UniformBuffer::SetData(const void* data, uint32_t size, uint32_t offset)
	{
		glNamedBufferSubData(m_RendererID, offset, size, data);
	}

}