#include "solpch.h"
#include "Buffer.h"

#include "Renderer.h"
#include "GalaxyDraw/Platform/OpenGL/OpenGL_VertexBuffer.h"
#include "GalaxyDraw/Platform/OpenGL/OpenGL_IndexBuffer.h"
#include "GalaxyDraw/Platform/OpenGL/OpenGL_InstanceBuffer.h"
#include <Sol/Core/Core.h>

namespace GalaxyDraw
{
	std::shared_ptr<VertexBuffer> VertexBuffer::Create(uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			SOL_CORE_ASSERT(false, "RendererAPI::None is not supported!");
			return nullptr;

		case RendererAPI::API::OpenGL: return std::make_shared<OpenGL_VertexBuffer>(size);
		}
		SOL_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
	std::shared_ptr <VertexBuffer> VertexBuffer::Create(float* verts, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:	
			SOL_CORE_ASSERT(false, "RendererAPI::None is not supported!");
			return nullptr;

		case RendererAPI::API::OpenGL: return std::make_shared<OpenGL_VertexBuffer>(verts, size);
		}
		SOL_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	std::shared_ptr <IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			SOL_CORE_ASSERT(false, "RendererAPI::None is not supported!");
			return nullptr;

		case RendererAPI::API::OpenGL: return std::make_shared<OpenGL_IndexBuffer>(indices, count);
		}
		SOL_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	std::shared_ptr <IndexBuffer> IndexBuffer::Create(const uint32_t* indices, uint32_t count)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			SOL_CORE_ASSERT(false, "RendererAPI::None is not supported!");
			return nullptr;

		case RendererAPI::API::OpenGL: return std::make_shared<OpenGL_IndexBuffer>(indices, count);
		}
		SOL_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}


	std::shared_ptr<InstanceBuffer> InstanceBuffer::Create(uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			SOL_CORE_ASSERT(false, "RendererAPI::None is not supported!");
			return nullptr;

		case RendererAPI::API::OpenGL: return std::make_shared<OpenGL_InstanceBuffer>(size);
		}
		SOL_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}


}