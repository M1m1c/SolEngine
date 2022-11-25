#include "solpch.h"
#include "Renderer3D.h"

#include "VertexArray.h"
#include "Shader.h"
#include "UniformBuffer.h"
#include "RenderCommand.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace GalaxyDraw 
{

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

	static Renderer3DData s_Data;

	void GalaxyDraw::Renderer3D::Init()
	{
	}

	void GalaxyDraw::Renderer3D::Shutdown()
	{
	}

	void GalaxyDraw::Renderer3D::BeginScene(const glm::mat4& projection, const glm::mat4& transform)
	{
	}

	void GalaxyDraw::Renderer3D::BeginScene(const OrthoCamera& camera)
	{
	}

	void GalaxyDraw::Renderer3D::EndScene()
	{
	}

	void GalaxyDraw::Renderer3D::Flush()
	{
	}

}
