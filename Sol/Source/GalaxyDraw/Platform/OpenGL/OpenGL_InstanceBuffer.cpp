#include "solpch.h"
#include "OpenGL_InstanceBuffer.h"

namespace GalaxyDraw 
{

	OpenGL_InstanceBuffer::OpenGL_InstanceBuffer(GLsizeiptr size, uint32_t stride, std::vector<VertexAttributeSpecs> specs, uint32_t layoutOffset)
	{
		GLCall(glGenBuffers(1, &ID));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, ID));
		GLCall(glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_STATIC_DRAW));
		SetVertexAttributes(stride, specs,layoutOffset);
	}

	OpenGL_InstanceBuffer::~OpenGL_InstanceBuffer()
	{
		GLCall(glDeleteBuffers(1, &ID));
	}

	void OpenGL_InstanceBuffer::Bind() const
	{
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, ID));
	}

	void OpenGL_InstanceBuffer::Unbind() const
	{
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
	}

	void OpenGL_InstanceBuffer::SetData(const void* data, uint32_t size)
	{
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, ID));
		GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, size, data));
	}

	void OpenGL_InstanceBuffer::SetVertexAttributes(uint32_t stride, std::vector<VertexAttributeSpecs> specs, uint32_t layoutOffset)
	{
		//TODO since this loop starts at 0 it overites the previously set layout locations in the shader,
		//we need to forawrd how many positions are in the vertex layout and increase both teh start and end index by that
		for (size_t i = layoutOffset; i < specs.size()+layoutOffset; i++)
		{
			auto specIndex = i - layoutOffset;
			auto& spec = specs[specIndex];

			//TODO in teh future we could try and add the glfloat and glfale parameter types to vertecsAttribureSpecs
			glVertexAttribPointer(i, spec.TypeSize, GL_FLOAT, GL_FALSE, stride, (void*)spec.PointerOffset);
			GLCall(glVertexAttribDivisor(i, 1));

			/*void glVertexAttribPointer(
				GLuint index,
				GLint size,
				GLenum type,
				GLboolean normalized,
				GLsizei stride,
				const void* pointer);*/
		}

		
		//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (void*)offsetof(InstanceData, color));
	}
}