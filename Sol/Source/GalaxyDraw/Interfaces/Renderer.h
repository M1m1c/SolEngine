#pragma once
#include "RendererAPI.h"
#include "GalaxyDraw/Camera.h"
#include "GalaxyDraw/shaderClass.h"
namespace GalaxyDraw 
{
	
	class Renderer
	{
	public:

		static void BeginScene(Camera& cam);
		static void EndScene();
		static void Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& va);

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	private:
		struct SceneData
		{
			glm::mat4 CameraMatrix;
		};

		static SceneData* s_SceneData;
	};

	
}