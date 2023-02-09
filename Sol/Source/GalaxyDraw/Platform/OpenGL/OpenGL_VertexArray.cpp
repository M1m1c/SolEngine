#include "solpch.h"

#include "OpenGL_VertexArray.h"
#include "GLMacros.h"
#include <glad/glad.h>

namespace GalaxyDraw
{
	//Temporary location
	static GLenum ShaderDataTypeToGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Float:		return GL_FLOAT;
		case ShaderDataType::Float2:	return GL_FLOAT;
		case ShaderDataType::Float3:	return GL_FLOAT;
		case ShaderDataType::Float4:	return GL_FLOAT;
		case ShaderDataType::Mat3:		return GL_FLOAT;
		case ShaderDataType::Mat4:		return GL_FLOAT;
		case ShaderDataType::Int:		return GL_INT;
		case ShaderDataType::Int2:		return GL_INT;
		case ShaderDataType::Int3:		return GL_INT;
		case ShaderDataType::Int4:		return GL_INT;
		case ShaderDataType::Bool:		return GL_BOOL;
		}
		SOL_CORE_ASSERT(false, "Unkown ShaderDataType!")
			return 0;
	}
	//Temporary location

	// Constructor that generates a VAO ID
	OpenGL_VertexArray::OpenGL_VertexArray()
	{
		GLCall(glGenVertexArrays(1, &ID));
	}

	OpenGL_VertexArray::OpenGL_VertexArray(const unsigned int vaoID) : ID(vaoID)
	{
	}

	OpenGL_VertexArray::~OpenGL_VertexArray()
	{
		GLCall(glDeleteVertexArrays(1, &ID));
	}

	// Links a VBO to the VAO using a certain layout
	void OpenGL_VertexArray::LinkAttrib(OpenGL_VertexBuffer& VBO, uint32_t layout, uint32_t numComponents, uint32_t type, khronos_ssize_t stride, void* offset)
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
	void OpenGL_VertexArray::Bind() const
	{
		GLCall(glBindVertexArray(ID));
	}

	// Unbinds the VAO
	void OpenGL_VertexArray::Unbind() const
	{
		GLCall(glBindVertexArray(0));
	}

	// Deletes the VAO
	void OpenGL_VertexArray::Delete()
	{
		GLCall(glDeleteVertexArrays(1, &ID));
	}

	void OpenGL_VertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vbo)
	{
		SOL_CORE_ASSERT(vbo->GetLayout().GetElements().size(), "VBO has no layout!");

		glBindVertexArray(ID);
		vbo->Bind();

		uint32_t index = 0;
		const auto& layout = vbo->GetLayout();
		for (const auto& element : layout)
		{
			glEnableVertexAttribArray(index);

			glVertexAttribPointer(index,
				element.GetComponentCount(),
				ShaderDataTypeToGLBaseType(element.Type),
				element.Normalized ? GL_TRUE : GL_FALSE,
				layout.GetStride(),
				(const void*)element.Offset);

			index++;
		}
		m_AttributeIndex = index;
		m_VertexBuffers.push_back(vbo);
	}

	void OpenGL_VertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& ebo) 
	{
		glBindVertexArray(ID);
		ebo->Bind();
		m_IndexBuffer = ebo;
	}

	//TODO maybe this should be more like the vertex buffer
	void OpenGL_VertexArray::SetInstanceBuffer(const std::shared_ptr<InstanceBuffer>& instanceBuffer) 
	{

		SOL_CORE_ASSERT(instanceBuffer->GetLayout().GetElements().size(), "instanceBuffer has no layout!");

		glBindVertexArray(ID);
		instanceBuffer->Bind();

		uint32_t index = m_AttributeIndex;
		const auto& layout = instanceBuffer->GetLayout();
		for (const auto& element : layout)
		{

			glVertexAttribPointer(index,
				element.GetComponentCount(),
				ShaderDataTypeToGLBaseType(element.Type),
				element.Normalized ? GL_TRUE : GL_FALSE,
				layout.GetStride(),
				(const void*)element.Offset);

			GLCall(glVertexAttribDivisor(index, 1)); //specifies that this is per instance
			glEnableVertexAttribArray(index);

			index++;
		}
		m_InstanceBuffer = instanceBuffer;
	}
}