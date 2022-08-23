#include"VBO.h"

// Constructor that generates a Vertex Buffer Object and links it to vertices
VBO::VBO(GLfloat* vertices, GLsizeiptr size)
{
	GLCall(glGenBuffers(1, &ID));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, ID));
	GLCall(glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW));
}

// Binds the VBO
void VBO::Bind() const
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, ID));
}

// Unbinds the VBO
void VBO::Unbind() const
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

// Deletes the VBO
void VBO::Delete()
{
	GLCall(glDeleteBuffers(1, &ID));
}