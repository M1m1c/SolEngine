#include "solpch.h"
#include "Framebuffer.h"
#include "Renderer.h"
#include "GalaxyDraw/Platform/OpenGL/OpenGL_Framebuffer.h"

namespace GalaxyDraw 
{
	std::unique_ptr<Framebuffer> Framebuffer::Create(const FramebufferProperties& properties)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			SOL_CORE_ASSERT(false, "RendererAPI::None is not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:		
				return std::make_unique<OpenGL_Framebuffer>(properties);
		}
		SOL_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}