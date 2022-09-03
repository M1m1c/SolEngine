#include "solpch.h"
#include "RenderCommand.h"
#include "GalaxyDraw/RenderScreen.h"

namespace GalaxyDraw 
{
	RendererAPI* RenderCommand::s_RendererAPI = new RenderScreen;
}