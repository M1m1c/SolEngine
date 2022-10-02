#include "solpch.h"
#include "Texture.h"

#include "Renderer.h"
#include "GalaxyDraw/Platform/OpenGL/OpenGL_Texture.h"

namespace GalaxyDraw {
	Sol::s_ptr<Texture2D> Texture2D::Create(uint32_t width, uint32_t height)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			SOL_CORE_ASSERT(false, "RendererAPI::None is not supported!");
			return nullptr;

		case RendererAPI::API::OpenGL: return std::make_shared<OpenGL_Texture2D>(width,height);
		}
		SOL_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;;
	}
	std::shared_ptr<Texture2D> Texture2D::Create(const std::string& path)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			SOL_CORE_ASSERT(false, "RendererAPI::None is not supported!");
			return nullptr;

		case RendererAPI::API::OpenGL: return std::make_shared<OpenGL_Texture2D>(path);
		}
		SOL_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}