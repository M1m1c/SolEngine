#include "solpch.h"
#include "ParticleSystem.h"
#include "Renderer.h"
#include "GalaxyDraw/Platform/OpenGL/OpenGL_ParticleSystem.h"

namespace GalaxyDraw 
{
    std::shared_ptr<ParticleSystem> GalaxyDraw::ParticleSystem::Create(const std::string& vertexFile, const std::string& fragmentFile, uint32_t maxParticles)
    {
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			SOL_CORE_ASSERT(false, "RendererAPI::None is not supported!");
			return nullptr;

		case RendererAPI::API::OpenGL:
				return std::make_shared<OpenGL_ParticleSystem>(vertexFile,fragmentFile, maxParticles);
			break;
		}
		SOL_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
    }
}

