#include "solpch.h"
#include "VertexArray.h"
#include "Renderer.h"
#include "GalaxyDraw/VAO.h"

namespace GalaxyDraw 
{
	VertexArray* VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			SOL_CORE_ASSERT(false, "RendererAPI::None is not supported!");
			return nullptr;

		case RendererAPI::API::OpenGL: return new VAO();
		}
		SOL_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}