#include"VAO.h"

// Constructor that generates a VAO ID
VAO::VAO()
{
	GLCall(glGenVertexArrays(1, &ID));
}

// Links a VBO to the VAO using a certain layout
void VAO::LinkAttrib(VBO& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset)
{
	VBO.Bind();
	
	// GL_FALSE here refers to normalization:
	GLCall(glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset));
	// normalized (from docs)
	// - For glVertexAttribPointer, specifies whether fixed - point data values should be normalized(GL_TRUE) or converted directly as fixed - point values(GL_FALSE) when they are accessed.

	GLCall(glEnableVertexAttribArray(layout));
	VBO.Unbind();
}

// Binds the VAO
void VAO::Bind() const
{
	GLCall(glBindVertexArray(ID));
}

// Unbinds the VAO
void VAO::Unbind() const
{
	GLCall(glBindVertexArray(0));
}

// Deletes the VAO
void VAO::Delete()
{
	GLCall(glDeleteVertexArrays(1, &ID));
}