#include "solpch.h"
#include "RenderingContext.h"

#include "Renderer.h"
#include "GalaxyDraw/Platform/OpenGL/OpenGL_Context.h"
#include <Sol/Core.h>
#include <GLFW/glfw3.h>

namespace GalaxyDraw
{
	std::shared_ptr<RenderingContext> RenderingContext::Create(GLFWwindow* windowHandle)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			SOL_CORE_ASSERT(false, "RendererAPI::None is not supported!");
			return nullptr;

		case RendererAPI::API::OpenGL: return std::make_shared<OpenGL_Context>(windowHandle);
		}
		SOL_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}
