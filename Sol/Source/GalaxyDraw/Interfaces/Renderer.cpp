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

	void Renderer::BeginScene(Camera& cam)
	{
		s_SceneData->CameraMatrix = cam.GetCameraMatrix();
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& va, const glm::mat4& transform)
	{
		shader->Bind();
		shader->setMat4("u_ViewProjection", s_SceneData->CameraMatrix);
		shader->setMat4("u_Transform", transform);

		va->Bind();
		RenderCommand::DrawIndexed(va);
	}

}