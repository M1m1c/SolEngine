#pragma once
#include <memory>
#include "Buffer.h"

namespace GalaxyDraw {

	class VertexArray
	{
	public:
		virtual ~VertexArray(){}
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vbo)  = 0;
		virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& ebo)  = 0;
		virtual void SetInstanceBuffer(const std::shared_ptr<InstanceBuffer>& instanceBuffer)  = 0;

		virtual const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const = 0;
		virtual const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const = 0;
		virtual const std::shared_ptr<InstanceBuffer>& GetInstanceBuffer() const = 0;


		//TODO turn into shared_ptr
		static std::shared_ptr <VertexArray> Create();

	};
}