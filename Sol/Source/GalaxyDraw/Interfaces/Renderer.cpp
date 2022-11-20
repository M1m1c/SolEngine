#include "solpch.h"

#include "Renderer.h"
#include "RenderCommand.h"

namespace GalaxyDraw 
{

	Renderer::SceneData* Renderer::s_SceneData = new Renderer::SceneData();

	void Renderer::Init()
	{
		RenderCommand::Init();
	}

	void Renderer::OnWindowResize(uint32_t width, uint32_t height)
	{
		RenderCommand::SetViewport(0, 0, width, height);
	}

	void Renderer::BeginScene(const glm::mat4& projection,const glm::mat4& transform)
	{
		glm::mat4 viewProj = projection * glm::inverse(transform);
		s_SceneData->CameraMatrix = viewProj;
	}

	void Renderer::BeginScene(OrthoCamera& cam)
	{
		s_SceneData->CameraMatrix = cam.GetCameraMatrix();
	}

	void Renderer::EndScene()
	{
		//Flush();
	}

	void Renderer::Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& va, const glm::mat4& transform)
	{
		shader->Bind();
		shader->setMat4("u_ViewProjection", s_SceneData->CameraMatrix);
		shader->setMat4("u_Transform", transform);

		va->Bind();
		RenderCommand::DrawIndexed(va);
	}

	void Renderer::DrawQuad(const glm::mat4& transform, const glm::vec4& color)
	{
		//TODO setup function

			//glm::vec3 pos(0.f, 0.f, 0.f);
			//glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos);

			//auto shader = m_ShaderLib.Get("Square");

			//GD_Renderer::Submit(shader, m_VertexArray, transform);

			//m_Texture->Bind();
	}

}