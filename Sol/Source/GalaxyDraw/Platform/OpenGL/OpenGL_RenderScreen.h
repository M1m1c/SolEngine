#pragma once

#include "GalaxyDraw/Interfaces/RendererAPI.h"

namespace GalaxyDraw 
{
	class OpenGL_VAO;
	class OpenGL_EBO;
	class Shader;
	class ModelOld;

	class OpenGL_RenderScreen : public RendererAPI
	{
	public:

		virtual void Init() override;

		void Draw(const OpenGL_VAO& va, const OpenGL_EBO& ib, const Shader& shader) const;
		void Draw(const ModelOld& model, const Shader& shader) const;

		virtual void DrawIndexed(const std::shared_ptr<VertexArray> va, uint32_t indexCount = 0) override;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
		virtual void SetClearColor(const glm::vec4& color) override;
		void Clear() const override;

		
	};
}