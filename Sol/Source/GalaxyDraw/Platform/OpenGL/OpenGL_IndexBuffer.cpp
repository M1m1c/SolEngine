#include "solpch.h"
#include "OpenGL_IndexBuffer.h"

#include "GLMacros.h"
#include <glad/glad.h>

namespace GalaxyDraw
{

	// Constructor that generates a Elements Buffer Object and links it to indices
	OpenGL_IndexBuffer::OpenGL_IndexBuffer(uint32_t* indicesArray, unsigned int elementCount) : count(elementCount)
	{
		GLCall(glGenBuffers(1, &ID));
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID));
		GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indicesArray, GL_STATIC_DRAW));
	}

	// Constructor that generates a Elements Buffer Object and links it to indices
	OpenGL_IndexBuffer::OpenGL_IndexBuffer(const uint32_t* indicesArray, unsigned int elementCount) : count(elementCount)
	{
		//TODO for some reason creating more than 1 buffer object names here,
		// prevents the program from crashing when generating a 3D meshes render data, why could that be?
		GLCall(glGenBuffers(2, &ID));
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID));
		GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indicesArray, GL_STATIC_DRAW));
	}

	// Destructor that hanldes deleting the buffer when this class gets deleted
	OpenGL_IndexBuffer::~OpenGL_IndexBuffer()
	{
		GLCall(glDeleteBuffers(1, &ID));
	}

	// Binds the EBO
	void OpenGL_IndexBuffer::Bind() const
	{
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID));
	}

	// Unbinds the EBO
	void OpenGL_IndexBuffer::Unbind() const
	{
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	}

	//TODO Maybe this is no longer needed when we have a destructor that does the same thing
	// Deletes the EBO
	void OpenGL_IndexBuffer::Delete()
	{
		GLCall(glDeleteBuffers(1, &ID));
	}
}