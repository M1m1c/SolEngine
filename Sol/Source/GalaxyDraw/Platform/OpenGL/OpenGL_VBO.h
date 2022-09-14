#ifndef VBO_CLASS_H
#define VBO_CLASS_H

#include <glad/glad.h>

#include "solpch.h"
#include "GLMacros.h"
#include "GalaxyDraw/Interfaces/Buffer.h"

namespace GalaxyDraw
{
	class OpenGL_VBO : public VertexBuffer
	{
	public:
		// Constructor that generates a Vertex Buffer Object and links it to vertices
		OpenGL_VBO(GLfloat* vertices, GLsizeiptr size);

		// Destructor that hanldes deleting the buffer when this class gets deleted
		virtual ~OpenGL_VBO();

		// Binds the VBO
		virtual void Bind() const override;
		// Unbinds the VBO
		virtual void Unbind() const override;
		// Deletes the VBO
		void Delete();

		virtual void SetLayout(const BufferLayout& layout) override { m_Layout = layout; }
		virtual const BufferLayout& GetLayout() const override { return m_Layout; }

	private:
		// Reference ID of the Vertex Buffer Object
		GLuint ID;
		BufferLayout m_Layout;
	};
}
#endif