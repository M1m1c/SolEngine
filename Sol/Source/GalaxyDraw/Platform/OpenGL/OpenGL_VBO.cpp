#include "solpch.h"

#include "OpenGL_VBO.h"

namespace GalaxyDraw
{
	// Constructor that generates a Vertex Buffer Object and links it to vertices
	OpenGL_VBO::OpenGL_VBO(GLsizeiptr size)
	{
		GLCall(glGenBuffers(1, &ID));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, ID));
		GLCall(glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW));
	}

	OpenGL_VBO::OpenGL_VBO(GLfloat* vertices, GLsizeiptr size)
	{
		GLCall(glGenBuffers(1, &ID));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, ID));
		GLCall(glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW));
	}

	// Destructor that hanldes deleting the buffer when this class gets deleted
	OpenGL_VBO::~OpenGL_VBO()
	{
		GLCall(glDeleteBuffers(1, &ID));
	}

	// Binds the VBO
	void OpenGL_VBO::Bind() const
	{
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, ID));
	}

	// Unbinds the VBO
	void OpenGL_VBO::Unbind() const
	{
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
	}

	//TODO Maybe this is no longer needed when we have a destructor that does the same thing
	// Deletes the VBO
	void OpenGL_VBO::Delete()
	{
		GLCall(glDeleteBuffers(1, &ID));
	}

	void OpenGL_VBO::SetData(const void* data, uint32_t size)
	{
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, ID));
		GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, size, data));
	}

}