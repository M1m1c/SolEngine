#ifndef VBO_CLASS_H
#define VBO_CLASS_H

#include <glad/glad.h>

#include "solpch.h"
#include "GLMacros.h"
#include "Buffer.h"

namespace GalaxyDraw
{
	class VBO : public VertexBuffer
	{
	public:
		// Reference ID of the Vertex Buffer Object
		GLuint ID;
		// Constructor that generates a Vertex Buffer Object and links it to vertices
		VBO(GLfloat* vertices, GLsizeiptr size);

		// Destructor that hanldes deleting the buffer when this class gets deleted
		virtual ~VBO();

		// Binds the VBO
		virtual void Bind() const override;
		// Unbinds the VBO
		virtual void Unbind() const override;
		// Deletes the VBO
		void Delete();
	};
}
#endif