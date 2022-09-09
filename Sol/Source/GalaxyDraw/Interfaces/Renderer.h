#pragma once
#include "RendererAPI.h"
#include "Shader.h"
#include "GalaxyDraw/Camera.h"
namespace GalaxyDraw 
{
	
	class Renderer
	{
	public:

		static void BeginScene(Camera& cam);
		static void EndScene();

		static void Submit(
			const std::shared_ptr<Shader>& shader,
			const std::shared_ptr<VertexArray>& va,
			const glm::mat4& transform = glm::mat4(1.f));

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	private:
		struct SceneData
		{
			glm::mat4 CameraMatrix;
		};

		static SceneData* s_SceneData;
	};

	
}