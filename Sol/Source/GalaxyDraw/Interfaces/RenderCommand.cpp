#include "solpch.h"
#include "RenderCommand.h"
#include "GalaxyDraw/Platform/OpenGL/OpenGL_RenderScreen.h"

namespace GalaxyDraw 
{
	RendererAPI* RenderCommand::s_RendererAPI = new OpenGL_RenderScreen;
}