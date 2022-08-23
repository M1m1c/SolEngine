#include"EBO.h"

// Constructor that generates a Elements Buffer Object and links it to indices
EBO::EBO(GLuint* indicesArray, unsigned int elementCount) : count (elementCount)
{
	GLCall(glGenBuffers(1, &ID));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLuint), indicesArray, GL_STATIC_DRAW));
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

// Deletes the EBO
void EBO::Delete()
{
	GLCall(glDeleteBuffers(1, &ID));
}