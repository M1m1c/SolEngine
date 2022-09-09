#include "solpch.h"

#include "RenderScreen.h"
#include <glad/glad.h>
#include "Platform/OpenGL/OpenGL_VAO.h"
#include "Platform/OpenGL/OpenGL_EBO.h"
#include "Interfaces/Shader.h"
#include "Model.h"

namespace GalaxyDraw
{

	void RenderScreen::Draw(const OpenGL_VAO& va, const OpenGL_EBO& ib, const Shader& shader) const
	{
		shader.Bind();
		va.Bind();
		ib.Bind();

		glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr);
	}

	void RenderScreen::Draw(const Model& model, const Shader& shader) const
	{
		for (size_t i = 0; i < model.meshes.size(); i++)
		{
			Mesh mesh = model.meshes[i];
			OpenGL_VAO vao(mesh.VAO);
			OpenGL_EBO ebo(&(mesh.indices[0]), model.meshes[i].indices.size());
			this->Draw(vao, ebo, shader);
		}
	}

	void RenderScreen::DrawIndexed(const std::shared_ptr<VertexArray> va)
	{
		glDrawElements(GL_TRIANGLES, va->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
	}

	void RenderScreen::SetClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void RenderScreen::Clear() const
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}


	

}