#ifndef EBO_CLASS_H
#define EBO_CLASS_H

#include<glad/glad.h>

#include "GLMacros.h"

class EBO
{
public:
	// ID reference of Elements Buffer Object
	GLuint ID;
	unsigned int count;
	// Constructor that generates a Elements Buffer Object and links it to indices
	EBO(GLuint* indices, unsigned int elementCount);

	// Binds the EBO
	void Bind() const;
	// Unbinds the EBO
	void Unbind() const;
	// Deletes the EBO
	void Delete();
};

#endif