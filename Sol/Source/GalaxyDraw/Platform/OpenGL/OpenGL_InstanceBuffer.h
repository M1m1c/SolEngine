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


		// Inherited via InstanceBuffer
		virtual void Bind() const override;

		virtual void Unbind() const override;


		// Inherited via InstanceBuffer
		virtual void SetData(const void* data, uint32_t size) override;

	private:
		// Reference ID of the Vertex Buffer Object
		GLuint ID;
	};

	
}
