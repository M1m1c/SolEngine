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

		Renderer3D::Statistics Stats;

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

		//TODO finish implementing, look at Renderer2D for reference

		s_Data.MissingTexture = Texture2D::Create(1, 1);
		uint32_t missingTextureData = 0xff00ff;
		s_Data.MissingTexture->SetData(&missingTextureData, sizeof(uint32_t));
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
		if (s_Data.MeshIndexCount)
		{
			uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.MeshVertexBufferPtr - (uint8_t*)s_Data.MeshVertexBufferBase);
			s_Data.MeshVertexBuffer->SetData(s_Data.MeshVertexBufferBase, dataSize);

			//// Bind textures
			//for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
			//	s_Data.TextureSlots[i]->Bind(i);

			s_Data.MeshShader->Bind();
			RenderCommand::DrawIndexed(s_Data.MeshVertexArray, s_Data.MeshIndexCount);
			s_Data.Stats.DrawCalls++;
		}

	}

	void Renderer3D::ResetStats()
	{
		memset(&s_Data.Stats, 0, sizeof(Statistics));
	}

	Renderer3D::Statistics Renderer3D::GetStats()
	{
		return s_Data.Stats;
	}

}
