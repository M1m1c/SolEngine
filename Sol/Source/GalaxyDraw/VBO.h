#ifndef VBO_CLASS_H
#define VBO_CLASS_H

#include<glad/glad.h>

#include "GLMacros.h"

class VBO
{
public:
	// Reference ID of the Vertex Buffer Object
	GLuint ID;
	// Constructor that generates a Vertex Buffer Object and links it to vertices
	VBO(GLfloat* vertices, GLsizeiptr size);

	// Binds the VBO
	void Bind() const;
	// Unbinds the VBO
	void Unbind() const;
	// Deletes the VBO
	void Delete();
};

#endif