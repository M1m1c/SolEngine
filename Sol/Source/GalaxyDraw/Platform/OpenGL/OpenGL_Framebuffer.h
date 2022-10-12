#pragma once

#include "GalaxyDraw/Interfaces/Framebuffer.h"

namespace GalaxyDraw
{
	class OpenGL_Framebuffer : public Framebuffer
	{
	public:
		OpenGL_Framebuffer(const FramebufferProperties& properties);
		virtual ~OpenGL_Framebuffer() override;

		void Invalidate();

		virtual void Bind() override;
		virtual void UnBind() override;
		virtual void Resize(uint32_t width, uint32_t height) override;
		virtual const FramebufferProperties& GetProperties() const override { return m_Properties; }
		virtual uint32_t GetColorAttachmentsRendererID() const override { return m_ColorAttachment; }

	private:
		uint32_t m_RendererID = 0;
		uint32_t m_ColorAttachment = 0;
		uint32_t m_DepthAttachment = 0;
		FramebufferProperties m_Properties;
	};


}