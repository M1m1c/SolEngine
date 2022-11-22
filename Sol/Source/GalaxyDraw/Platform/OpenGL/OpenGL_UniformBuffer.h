#pragma once

#include "GalaxyDraw/Interfaces/UniformBuffer.h"

namespace GalaxyDraw {

	class OpenGL_UniformBuffer : public UniformBuffer
	{
	public:
		OpenGL_UniformBuffer(uint32_t size, uint32_t binding);
		virtual ~OpenGL_UniformBuffer();

		virtual void SetData(const void* data, uint32_t size, uint32_t offset = 0) override;
	private:
		uint32_t m_RendererID = 0;
	};
}