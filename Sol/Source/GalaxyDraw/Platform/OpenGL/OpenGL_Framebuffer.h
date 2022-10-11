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
		virtual const FramebufferProperties& GetProperties() const override { return m_Properties; }
		virtual uint32_t GetColorAttachmentsRendererID() const override { return m_ColorAttachment; }

	private:
		uint32_t m_RendererID;
		uint32_t m_ColorAttachment;
		uint32_t m_DepthAttachment;
		FramebufferProperties m_Properties;
	};

	
}