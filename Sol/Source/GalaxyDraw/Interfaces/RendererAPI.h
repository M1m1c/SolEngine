#pragma once

#include "glm/glm.hpp"
#include "VertexArray.h"
#include "Buffer.h"
#include "Shader.h"
#include "GalaxyDraw/ModelOld.h"

namespace GalaxyDraw 
{
	class RendererAPI
	{
	public:
		enum class API
		{
			None = 0,
			OpenGL = 1

		};

		virtual void Init() = 0;

		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;

		virtual void SetClearColor(const glm::vec4& color) = 0;

		//TODO Add a DrawInstanced funciton that ends up using glDrawArraysInstanced(GL_TRIANGLES, 0, numVertices, (GLsizei)instanceData.size());
		virtual void DrawInstanced(const std::shared_ptr<VertexArray> va, int32_t instanceCount) = 0;
		virtual void DrawIndexed(const std::shared_ptr<VertexArray> va, uint32_t indexCount = 0) = 0;
		/*virtual void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const =0;
		virtual void Draw(const Model& model, const Shader& shader) const=0;*/
		virtual void Clear() const=0;

		inline static API GetAPI() { return s_API; }

	private:
		static API s_API;
	};
}
