#include "solpch.h"
#include "VertexArray.h"
#include "Renderer.h"
#include "VAO.h"

namespace GalaxyDraw 
{
	VertexArray* VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::None:
			SOL_CORE_ASSERT(false, "RendererAPI::None is not supported!");
			return nullptr;

		case RendererAPI::OpenGL: return new VAO();
		}
		SOL_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}