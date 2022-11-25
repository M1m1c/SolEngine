#include "solpch.h"
#include "Renderer.h"
#include "Renderer2D.h"
#include "RenderCommand.h"
#include <GalaxyDraw/Interfaces/UniformBuffer.h>

namespace GalaxyDraw {

	struct Renderer3DData
	{
		static const uint32_t MaxMeshes = 20000;
		static const uint32_t MaxVertices = MaxMeshes * 4;
		static const uint32_t MaxIndices = MaxMeshes * 6;
		static const uint32_t MaxTextureSlots = 32; // TODO: RenderCaps

		std::shared_ptr<VertexArray> MeshVertexArray;
		std::shared_ptr<VertexBuffer> MeshVertexBuffer;
		std::shared_ptr<Shader> MeshShader;
		std::shared_ptr<Texture2D> MissingTexture;

		uint32_t MeshIndexCount = 0;
		Vertex* MeshVertexBufferBase = nullptr;
		Vertex* MeshVertexBufferPtr = nullptr;

		struct CameraData
		{
			glm::mat4 ViewProjection;
		};
		CameraData CameraBuffer;
		std::shared_ptr<UniformBuffer> CameraUniformBuffer;
	};

	std::unique_ptr<Renderer::SceneData> Renderer::s_SceneData = std::make_unique<Renderer::SceneData>();

	static Renderer3DData s_Data;

	void Renderer::Init()
	{
		SOL_PROFILE_FUNCTION();

		RenderCommand::Init();
		Renderer2D::Init();
	}

	void Renderer::Shutdown()
	{
		Renderer2D::Shutdown();
	}

	void Renderer::OnWindowResize(uint32_t width, uint32_t height)
	{
		RenderCommand::SetViewport(0, 0, width, height);
	}

	void Renderer::BeginScene(OrthoCamera& camera)
	{
		s_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray, const glm::mat4& transform)
	{
		shader->Bind();
		shader->SetMat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);
		shader->SetMat4("u_Transform", transform);

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}

}