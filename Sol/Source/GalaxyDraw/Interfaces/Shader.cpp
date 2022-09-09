#include "solpch.h"

#include "Shader.h"
#include "Renderer.h"
#include "GalaxyDraw/Platform/OpenGL/OpenGLShader.h"

namespace GalaxyDraw 
{
	Shader* Shader::Create(const char* vertexFile, const char* fragmentFile)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			SOL_CORE_ASSERT(false, "RendererAPI::None is not supported!");
			return nullptr;

		case RendererAPI::API::OpenGL: return new OpenGLShader(vertexFile, fragmentFile);
		}
		SOL_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
		return nullptr;
	}
}