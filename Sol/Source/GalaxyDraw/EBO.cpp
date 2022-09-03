#include "solpch.h"
#include "EBO.h"

#include "GLMacros.h"
#include <glad/glad.h>

namespace GalaxyDraw
{

	// Constructor that generates a Elements Buffer Object and links it to indices
	EBO::EBO(uint32_t* indicesArray, unsigned int elementCount) : count(elementCount)
	{
		GLCall(glGenBuffers(1, &ID));
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID));
		GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indicesArray, GL_STATIC_DRAW));
	}

	// Destructor that hanldes deleting the buffer when this class gets deleted
	EBO::~EBO()
	{
		GLCall(glDeleteBuffers(1, &ID));
	}

	// Binds the EBO
	void EBO::Bind() const
	{
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID));
	}

	// Unbinds the EBO
	void EBO::Unbind() const
	{
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	}

	//TODO Maybe this is no longer needed when we have a destructor that does the same thing
	// Deletes the EBO
	void EBO::Delete()
	{
		GLCall(glDeleteBuffers(1, &ID));
	}
}