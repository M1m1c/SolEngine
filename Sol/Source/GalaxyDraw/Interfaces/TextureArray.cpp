#include "solpch.h"
#include "TextureArray.h"

#include "Renderer.h"
#include "GalaxyDraw/Platform/OpenGL/OpenGL_TextureArray.h"


namespace GalaxyDraw {
	Sol::s_ptr<TextureArray> TextureArray::Create(uint32_t widthHeight, uint32_t textureUnit)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			SOL_CORE_ASSERT(false, "RendererAPI::None is not supported!");
			return nullptr;

		case RendererAPI::API::OpenGL: return std::make_shared<OpenGL_TextureArray>(widthHeight, textureUnit);
		}
		SOL_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}
