#include "solpch.h"

#include "OpenGL_RenderScreen.h"
#include <glad/glad.h>
#include "OpenGL_VAO.h"
#include "OpenGL_EBO.h"
#include "GalaxyDraw/Interfaces/Shader.h"
#include "GalaxyDraw/Model.h"

namespace GalaxyDraw
{
	void OpenGL_RenderScreen::Init()
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void OpenGL_RenderScreen::Draw(const OpenGL_VAO& va, const OpenGL_EBO& ib, const Shader& shader) const
	{
		shader.Bind();
		va.Bind();
		ib.Bind();

		glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr);
	}

	void OpenGL_RenderScreen::Draw(const Model& model, const Shader& shader) const
	{
		for (size_t i = 0; i < model.meshes.size(); i++)
		{
			Mesh mesh = model.meshes[i];
			OpenGL_VAO vao(mesh.VAO);
			OpenGL_EBO ebo(&(mesh.indices[0]), model.meshes[i].indices.size());
			this->Draw(vao, ebo, shader);
		}
	}

	void OpenGL_RenderScreen::DrawIndexed(const std::shared_ptr<VertexArray> va, uint32_t indexCount)
	{
		uint32_t count = indexCount == 0 ? va->GetIndexBuffer()->GetCount() : indexCount;
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
	}

	void OpenGL_RenderScreen::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		glViewport(x, y, width, height);
	}

	void OpenGL_RenderScreen::SetClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGL_RenderScreen::Clear() const
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}


	

}