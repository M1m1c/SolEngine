#pragma once

#include "GalaxyDraw/Interfaces/Buffer.h"

#include <glad/glad.h>
#include "GLMacros.h"

namespace GalaxyDraw 
{
	class OpenGL_InstanceBuffer : public InstanceBuffer
	{
	public:
		OpenGL_InstanceBuffer(GLsizeiptr size);
		~OpenGL_InstanceBuffer();


		virtual void Bind() const override;

		virtual void Unbind() const override;

		virtual void SetData(const void* data, uint32_t size) override;

		virtual void SetLayout(const BufferLayout& layout) override { m_Layout = layout; }
		virtual const BufferLayout& GetLayout() const override { return m_Layout; }

	private:
		// Reference ID of the Vertex Buffer Object
		GLuint ID;
		BufferLayout m_Layout;
	};

	
}
