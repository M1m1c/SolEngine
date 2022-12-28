#include "solpch.h"
#include "VertexArray.h"
#include "Renderer.h"
#include "GalaxyDraw/Platform/OpenGL/OpenGL_VertexArray.h"

namespace GalaxyDraw 
{
	std::shared_ptr <VertexArray> VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			SOL_CORE_ASSERT(false, "RendererAPI::None is not supported!");
			return nullptr;

		case RendererAPI::API::OpenGL: return std::make_shared<OpenGL_VertexArray>();
		}
		SOL_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}