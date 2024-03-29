#pragma once

#include "RendererAPI.h"	

namespace GalaxyDraw {
	class RenderCommand
	{
	public:

		inline static void Init()
		{
			s_RendererAPI->Init();
		}

		inline static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
		{
			s_RendererAPI->SetViewport(x, y, width, height);
		}

		inline static void SetClearColor(const glm::vec4& color)
		{
			s_RendererAPI->SetClearColor(color);
		}

		inline static void Clear()
		{
			s_RendererAPI->Clear();
		}

		inline static void DrawIndexed(const std::shared_ptr<VertexArray>& va, uint32_t indexCount = 0)
		{
			s_RendererAPI->DrawIndexed(va, indexCount);
		}

		//TODO finialize this
		inline static void DrawInstanced(const std::shared_ptr<VertexArray>& va, int32_t instanceCount)
		{
			s_RendererAPI->DrawInstanced(va,instanceCount);
		}
	private:
		static RendererAPI* s_RendererAPI;
	};


}