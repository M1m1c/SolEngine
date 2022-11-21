#pragma once
#include "RendererAPI.h"
#include "Shader.h"
#include "OrthoCamera.h"
namespace GalaxyDraw 
{

	struct QuadVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoord;
		float TexIndex;
		float TilingFactor;

		// Editor-only
		int EntityID;
	};

	struct Renderer2DData
	{
		static const uint32_t MaxQuads = 20000;
		static const uint32_t MaxVertices = MaxQuads * 4;
		static const uint32_t MaxIndices = MaxQuads * 6;
		static const uint32_t MaxTextureSlots = 32;

		std::shared_ptr<VertexArray> QuadVertexArray;
		std::shared_ptr<VertexBuffer> QuadVertexBuffer;
		std::shared_ptr<Shader> QuadShader;
		std::shared_ptr<class Texture2D> WhiteTexture;

		uint32_t QuadIndexCount = 0;
		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexBufferPtr = nullptr;

		glm::vec4 QuadVertexPositions[4];

		std::array<std::shared_ptr<class Texture2D>, MaxTextureSlots> TextureSlots;
		uint32_t TextureSlotIndex = 1; // 0 = white texture

	};
	
	class Renderer
	{
	public:

		static void Init();

		static void OnWindowResize(uint32_t width, uint32_t height);

		static void BeginScene(const glm::mat4& projection, const glm::mat4& transform);
		static void BeginScene(OrthoCamera& cam); //TODO remove
		static void EndScene();

		static void Submit(
			const std::shared_ptr<Shader>& shader,
			const std::shared_ptr<VertexArray>& va,
			const glm::mat4& transform = glm::mat4(1.f));


		static void StartBatch();

		static void Flush();

		static void NextBatch();

		static void DrawQuad(const glm::mat4& transform, const glm::vec4& color);

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	private:
		struct SceneData
		{
			glm::mat4 CameraMatrix;
		};

		static SceneData* s_SceneData;

	/*	static Renderer2DData s_2DData;

		static ShaderLibrary s_ShaderLib;*/
	};


	
}