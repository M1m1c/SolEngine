#include "solpch.h"

#include "Renderer.h"
#include "RenderCommand.h"

namespace GalaxyDraw 
{
	void Renderer::BeginScene()
	{
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::Submit(const std::shared_ptr<VertexArray>& va)
	{
		va->Bind();
		RenderCommand::DrawIndexed(va);
	}

}