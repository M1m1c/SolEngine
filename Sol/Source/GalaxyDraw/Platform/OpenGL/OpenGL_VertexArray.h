#ifndef VAO_CLASS_H
#define VAO_CLASS_H


#include "GalaxyDraw/Interfaces/VertexArray.h"
#include "OpenGL_VertexBuffer.h"

namespace GalaxyDraw 
{

	class OpenGL_VertexArray : public VertexArray
	{
	public:
		// ID reference for the Vertex Array Object
		uint32_t ID;
		// Constructor that generates a VAO ID
		OpenGL_VertexArray();
		OpenGL_VertexArray(const unsigned int vaoID);
		virtual ~OpenGL_VertexArray();

		// Links a VBO Attribute such as a position or color to the VAO
		void LinkAttrib(OpenGL_VertexBuffer& VBO, uint32_t layout, uint32_t numComponents, uint32_t type, khronos_ssize_t stride, void* offset);
		// Binds the VAO
		virtual void Bind() const override;
		// Unbinds the VAO
		virtual void Unbind() const override;
		// Deletes the VAO
		void Delete();

		//TODO maybe we should change to SetVertexBuffer() when we go for instanced rendering
		virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vbo)  override;
		virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& ebo) override;
		//TODO maybe the instance buffer should be what the AddVertexBuffer() is, 
		// meaning a collection of changable instance data.
		virtual void SetInstanceBuffer(const std::shared_ptr<InstanceBuffer>& instanceBuffer) override;

		virtual const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const override { return m_VertexBuffers; }
		virtual const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const override { return m_IndexBuffer; }
		virtual const std::shared_ptr<InstanceBuffer>& GetInstanceBuffer() const override { return m_InstanceBuffer; }

	private:
		std::vector<std::shared_ptr<VertexBuffer>> m_VertexBuffers;
		std::shared_ptr<IndexBuffer> m_IndexBuffer;
		std::shared_ptr<InstanceBuffer> m_InstanceBuffer;
		uint32_t m_AttributeIndex = 0;
	};

}
#endif#pragma once
