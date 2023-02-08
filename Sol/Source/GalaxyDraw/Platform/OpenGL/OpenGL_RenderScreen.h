#pragma once

#include "GalaxyDraw/Interfaces/RendererAPI.h"

namespace GalaxyDraw 
{
	class OpenGL_VertexArray;
	class OpenGL_IndexBuffer;
	class Shader;
	class ModelOld;

	class OpenGL_RenderScreen : public RendererAPI
	{
	public:

		virtual void Init() override;

		void Draw(const OpenGL_VertexArray& va, const OpenGL_IndexBuffer& ib, const Shader& shader) const;
		void Draw(const ModelOld& model, const Shader& shader) const;


		virtual void DrawInstanced(const std::shared_ptr<VertexArray> va, int32_t instanceCount) override;
		virtual void DrawIndexed(const std::shared_ptr<VertexArray> va, uint32_t indexCount = 0) override;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
		virtual void SetClearColor(const glm::vec4& color) override;
		void Clear() const override;

		
	};
}