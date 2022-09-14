#pragma once

#include "GalaxyDraw/Interfaces/RendererAPI.h"

namespace GalaxyDraw 
{
	class OpenGL_VAO;
	class OpenGL_EBO;
	class Shader;
	class Model;

	class OpenGL_RenderScreen : public RendererAPI
	{
	public:

		virtual void Init() override;

		void Draw(const OpenGL_VAO& va, const OpenGL_EBO& ib, const Shader& shader) const;
		void Draw(const Model& model, const Shader& shader) const;

		virtual void DrawIndexed(const std::shared_ptr<VertexArray> va) override;
		virtual void SetClearColor(const glm::vec4& color) override;
		void Clear() const override;

		
	};
}