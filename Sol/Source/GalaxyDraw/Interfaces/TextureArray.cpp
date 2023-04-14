#include "solpch.h"
#include "TextureArray.h"

#include "Renderer.h"
#include "GalaxyDraw/Platform/OpenGL/OpenGL_Texture.h"


namespace GalaxyDraw {
	Sol::s_ptr<TextureArray> TextureArray::Create(uint32_t numTextures)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			SOL_CORE_ASSERT(false, "RendererAPI::None is not supported!");
			return nullptr;

		//case RendererAPI::API::OpenGL: return std::make_shared<OpenGL_TextureArray>(numTextures);
		}
		SOL_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}
