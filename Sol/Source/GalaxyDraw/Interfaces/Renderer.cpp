#include "solpch.h"

#include "Renderer.h"
#include "RenderCommand.h"
#include "Texture.h"

namespace GalaxyDraw 
{

	Renderer::SceneData* Renderer::s_SceneData = new Renderer::SceneData();
	static Renderer2DData s_2DData;
	static ShaderLibrary s_ShaderLib;

	void Renderer::Init()
	{
		RenderCommand::Init();

		s_2DData.QuadVertexArray = VertexArray::Create();

		s_2DData.QuadVertexBuffer = VertexBuffer::Create(s_2DData.MaxVertices * sizeof(QuadVertex));
		s_2DData.QuadVertexBuffer->SetLayout({
			{ShaderDataType::Float3, "a_Position"},
			{ShaderDataType::Float2, "a_TexCoord"},
			/*{ ShaderDataType::Float3, "a_Position"     },
			{ ShaderDataType::Float4, "a_Color"        },
			{ ShaderDataType::Float2, "a_TexCoord"     },
			{ ShaderDataType::Float,  "a_TexIndex"     },
			{ ShaderDataType::Float,  "a_TilingFactor" },
			{ ShaderDataType::Int,    "a_EntityID"     }*/
			});
		s_2DData.QuadVertexArray->AddVertexBuffer(s_2DData.QuadVertexBuffer);

		s_2DData.QuadVertexBufferBase = new QuadVertex[s_2DData.MaxVertices];

		uint32_t* quadIndices = new uint32_t[s_2DData.MaxIndices];

		uint32_t offset = 0;
		for (uint32_t i = 0; i < s_2DData.MaxIndices; i += 6)
		{
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;

			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;

			offset += 4;
		}

		std::shared_ptr<IndexBuffer> quadIB = IndexBuffer::Create(quadIndices, s_2DData.MaxIndices);

		//TODO for some reason teh indecies odn't match here figure out why
	/*	uint32_t indices[6] = { 0,1,2,2,3,0 };
		auto indexBuffer = IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));*/

		s_2DData.QuadVertexArray->SetIndexBuffer(quadIB);
		delete[] quadIndices;

		s_2DData.QuadShader = s_ShaderLib.Load("Square", "Square.vert", "Square.frag");

		s_2DData.QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		s_2DData.QuadVertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
		s_2DData.QuadVertexPositions[2] = { 0.5f,  0.5f, 0.0f, 1.0f };
		s_2DData.QuadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };


		s_2DData.WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_2DData.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));
		s_2DData.TextureSlots[0] = s_2DData.WhiteTexture;

	}

	void Renderer::OnWindowResize(uint32_t width, uint32_t height)
	{
		RenderCommand::SetViewport(0, 0, width, height);
	}

	void Renderer::BeginScene(const glm::mat4& projection,const glm::mat4& transform)
	{
		glm::mat4 viewProj = projection * glm::inverse(transform);
		s_SceneData->CameraMatrix = viewProj;

		//TODO implement this
		//s_2DData.CameraUniformBuffer->SetData(&s_Data.CameraBuffer, sizeof(Renderer2DData::CameraData));

		StartBatch();
	}

	//TODO remove this old way of begining scene
	/*void Renderer::BeginScene(OrthoCamera& cam)
	{
		s_SceneData->CameraMatrix = cam.GetCameraMatrix();
		StartBatch();
	}*/

	void Renderer::EndScene()
	{
		Flush();
	}

	//TODO remove old way of manual drawing, this is replaced by teh flush function
	/*void Renderer::Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& va, const glm::mat4& transform)
	{
		shader->Bind();
		shader->setMat4("u_ViewProjection", s_SceneData->CameraMatrix);
		shader->setMat4("u_Transform", transform);

		va->Bind();
		RenderCommand::DrawIndexed(va);
	}*/

	void Renderer::StartBatch()
	{
		s_2DData.QuadIndexCount = 0;
		s_2DData.QuadVertexBufferPtr = s_2DData.QuadVertexBufferBase;

		s_2DData.TextureSlotIndex = 1;
	}

	void Renderer::Flush()
	{
		if (s_2DData.QuadIndexCount)
		{
			uint32_t dataSize = (uint32_t)((uint8_t*)s_2DData.QuadVertexBufferPtr - (uint8_t*)s_2DData.QuadVertexBufferBase);
			s_2DData.QuadVertexBuffer->SetData(s_2DData.QuadVertexBufferBase, dataSize);

			// Bind textures
			for (uint32_t i = 0; i < s_2DData.TextureSlotIndex; i++)
				s_2DData.TextureSlots[i]->Bind(i);

			s_2DData.QuadShader->Bind();

			//TODO replace this with a uniform buffer, also update shader to use uniform buffer
			s_2DData.QuadShader->setMat4("u_ViewProjection", s_SceneData->CameraMatrix);
			s_2DData.QuadShader->setMat4("u_Transform", glm::mat4(1.0f));

			RenderCommand::DrawIndexed(s_2DData.QuadVertexArray, s_2DData.QuadIndexCount);
			//s_2DData.Stats.DrawCalls++;
		}

	}

	void Renderer::NextBatch()
	{
		Flush();
		StartBatch();
	}

	//TODO change to draw cube
	void Renderer::DrawQuad(const glm::mat4& transform, const glm::vec4& color)
	{
		//TODO setup function
		SOL_PROFILE_FUNCTION();

		constexpr size_t quadVertexCount = 4;
		const float textureIndex = 0.0f; // White Texture
		constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
		const float tilingFactor = 1.0f;

		if (s_2DData.QuadIndexCount >= Renderer2DData::MaxIndices)
			NextBatch();

		for (size_t i = 0; i < quadVertexCount; i++)
		{
			s_2DData.QuadVertexBufferPtr->Position = transform * s_2DData.QuadVertexPositions[i];
			s_2DData.QuadVertexBufferPtr->Color = color;
			s_2DData.QuadVertexBufferPtr->TexCoord = textureCoords[i];
			s_2DData.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_2DData.QuadVertexBufferPtr->TilingFactor = tilingFactor;
			//s_2DData.QuadVertexBufferPtr->EntityID = entityID;
			s_2DData.QuadVertexBufferPtr++;
		}

		s_2DData.QuadIndexCount += 6;

		//s_2DData.Stats.QuadCount++;
	}

}