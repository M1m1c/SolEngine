#include "solpch.h"
#include "Buffer.h"

#include "Renderer.h"
#include "GalaxyDraw/Platform/OpenGL/OpenGL_VBO.h"
#include "GalaxyDraw/Platform/OpenGL/OpenGL_EBO.h"
#include <Sol/Core.h>

namespace GalaxyDraw
{
	VertexBuffer* VertexBuffer::Create(float* verts, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:	
			SOL_CORE_ASSERT(false, "RendererAPI::None is not supported!");
			return nullptr;

		case RendererAPI::API::OpenGL: return new OpenGL_VBO(verts, size);
		}
		SOL_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t count)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			SOL_CORE_ASSERT(false, "RendererAPI::None is not supported!");
			return nullptr;

		case RendererAPI::API::OpenGL: return new OpenGL_EBO(indices, count);
		}
		SOL_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}


}