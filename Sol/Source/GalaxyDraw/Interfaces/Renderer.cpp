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

	void Renderer::BeginScene(Camera& cam)
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

}