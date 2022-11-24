#include "solpch.h"
#include "Model.h"
#include "Renderer.h"
#include "GalaxyDraw/Platform/OpenGL/OpenGL_Model.h"

namespace GalaxyDraw 
{
	std::shared_ptr<Model> Model::Create(const std::string& path)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			SOL_CORE_ASSERT(false, "RendererAPI::None is not supported!");
			return nullptr;

		case RendererAPI::API::OpenGL: return std::make_shared<OpenGL_Model>(path);
		}
		SOL_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}

