#pragma once

#include "Interfaces/RendererAPI.h"

namespace GalaxyDraw 
{
	class VAO;
	class EBO;
	class Shader;
	class Model;

	class RenderScreen : public RendererAPI
	{
	public:
		void Draw(const VAO& va, const EBO& ib, const Shader& shader) const;
		void Draw(const Model& model, const Shader& shader) const;

		virtual void DrawIndexed(const std::shared_ptr<VertexArray> va) override;
		virtual void SetClearColor(const glm::vec4& color) override;
		void Clear() const override;

		
	};
}