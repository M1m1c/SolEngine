#include "solpch.h"

#include "OpenGL_RenderScreen.h"
#include <glad/glad.h>
#include "OpenGL_VertexArray.h"
#include "OpenGL_IndexBuffer.h"
#include "GalaxyDraw/Interfaces/Shader.h"
#include "GalaxyDraw/ModelOld.h"

namespace GalaxyDraw
{
	void OpenGL_RenderScreen::Init()
	{
		glEnable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void OpenGL_RenderScreen::Draw(const OpenGL_VertexArray& va, const OpenGL_IndexBuffer& ib, const Shader& shader) const
	{
		shader.Bind();
		va.Bind();
		ib.Bind();

		glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr);
	}

	void OpenGL_RenderScreen::Draw(const ModelOld& model, const Shader& shader) const
	{
		for (size_t i = 0; i < model.meshes.size(); i++)
		{
			MeshOld mesh = model.meshes[i];
			OpenGL_VertexArray vao(mesh.VAO);
			OpenGL_IndexBuffer ebo(&(mesh.indices[0]), model.meshes[i].indices.size());
			this->Draw(vao, ebo, shader);
		}
	}

	void OpenGL_RenderScreen::DrawInstanced(const std::shared_ptr<VertexArray> va, int32_t instanceCount)
	{
		va->Bind();
		uint32_t count = va->GetIndexBuffer()->GetCount();
		glDrawElementsInstanced(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr, instanceCount);
	}

	void OpenGL_RenderScreen::DrawIndexed(const std::shared_ptr<VertexArray> va, uint32_t indexCount)
	{
		va->Bind();
		uint32_t count = indexCount ? indexCount : va->GetIndexBuffer()->GetCount();
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