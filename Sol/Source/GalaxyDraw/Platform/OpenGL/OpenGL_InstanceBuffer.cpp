#include "solpch.h"
#include "OpenGL_InstanceBuffer.h"

namespace GalaxyDraw 
{

	OpenGL_InstanceBuffer::OpenGL_InstanceBuffer(GLsizeiptr size)
	{
		GLCall(glGenBuffers(1, &ID));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, ID));
		GLCall(glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_STATIC_DRAW));
	}

	OpenGL_InstanceBuffer::~OpenGL_InstanceBuffer()
	{
		GLCall(glDeleteBuffers(1, &ID));
	}

	void OpenGL_InstanceBuffer::Bind() const
	{
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, ID));
	}

	void OpenGL_InstanceBuffer::Unbind() const
	{
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
	}

	void OpenGL_InstanceBuffer::SetData(const void* data, uint32_t size)
	{
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, ID));
		GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, size, data));
	}
}