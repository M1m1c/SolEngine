#include "solpch.h"
#include "UniformBuffer.h"

#include "GalaxyDraw/interfaces/Renderer.h"
#include "GalaxyDraw/Platform/OpenGL/OpenGL_UniformBuffer.h"

namespace GalaxyDraw {

	std::shared_ptr<UniformBuffer> UniformBuffer::Create(uint32_t size, uint32_t binding)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    SOL_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return std::make_shared<OpenGL_UniformBuffer>(size, binding);
		}

		SOL_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}