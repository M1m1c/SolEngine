#include "solpch.h"

#include "RenderScreen.h"

namespace GalaxyDraw
{

	void RenderScreen::Draw(const VAO& va, const EBO& ib, const Shader& shader) const
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
			VAO vao(mesh.VAO);
			EBO ebo(&(mesh.indices[0]), model.meshes[i].indices.size());
			this->Draw(vao, ebo, shader);
		}
	}

	void RenderScreen::Clear() const
	{
		glClear(GL_COLOR_BUFFER_BIT);
	}

}