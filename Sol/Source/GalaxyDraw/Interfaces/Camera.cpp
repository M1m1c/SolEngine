#include "solpch.h"
#include "Camera.h"

#include "Renderer.h"
#include "GalaxyDraw/Platform/OpenGL/OpenGL_Camera.h"

namespace GalaxyDraw 
{
	std::shared_ptr<Camera> Camera::Create(int width, int height, glm::vec2 aspectRatio, glm::vec3 position)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			SOL_CORE_ASSERT(false, "RendererAPI::None is not supported!");
			return nullptr;

		case RendererAPI::API::OpenGL:			
				return std::make_shared<OpenGL_Camera>(width,height,aspectRatio,position);
		}
		SOL_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}