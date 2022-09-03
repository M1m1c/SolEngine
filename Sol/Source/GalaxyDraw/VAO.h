#ifndef VAO_CLASS_H
#define VAO_CLASS_H


#include "Interfaces/VertexArray.h"
#include "VBO.h"

namespace GalaxyDraw 
{

	class VAO : public VertexArray
	{
	public:
		// ID reference for the Vertex Array Object
		uint32_t ID;
		// Constructor that generates a VAO ID
		VAO();
		VAO(const unsigned int vaoID);
		virtual ~VAO();

		// Links a VBO Attribute such as a position or color to the VAO
		void LinkAttrib(VBO& VBO, uint32_t layout, uint32_t numComponents, uint32_t type, khronos_ssize_t stride, void* offset);
		// Binds the VAO
		virtual void Bind() const override;
		// Unbinds the VAO
		virtual void Unbind() const override;
		// Deletes the VAO
		void Delete();

		virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vbo)  override;
		virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& ebo) override;

		virtual const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const override { return m_VertexBuffers; }
		virtual const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const override { return m_IndexBuffer; }

	private:
		std::vector<std::shared_ptr<VertexBuffer>> m_VertexBuffers;
		std::shared_ptr<IndexBuffer> m_IndexBuffer;
	};

}
#endif#pragma once
