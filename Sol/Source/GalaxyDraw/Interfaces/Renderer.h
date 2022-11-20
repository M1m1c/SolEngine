#pragma once
#include "RendererAPI.h"
#include "Shader.h"
#include "OrthoCamera.h"
namespace GalaxyDraw 
{
	
	class Renderer
	{
	public:

		static void Init();

		static void OnWindowResize(uint32_t width, uint32_t height);

		static void BeginScene(const glm::mat4& projection, const glm::mat4& transform);
		static void BeginScene(OrthoCamera& cam);
		static void EndScene();

		static void Submit(
			const std::shared_ptr<Shader>& shader,
			const std::shared_ptr<VertexArray>& va,
			const glm::mat4& transform = glm::mat4(1.f));


		static void DrawQuad(const glm::mat4& transform, const glm::vec4& color);

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	private:
		struct SceneData
		{
			glm::mat4 CameraMatrix;
		};

		static SceneData* s_SceneData;
	};

	
}