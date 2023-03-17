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
		virtual uint32_t GetColorAttachmentsRendererID(uint32_t index=0) const override 
		{ 
			SOL_CORE_ASSERT("Index out of bounds",index < m_ColorAttachments.size());
			return m_ColorAttachments[index]; 
		}

	private:
		uint32_t m_RendererID = 0;
		//uint32_t m_ColorAttachment = 0;
		//uint32_t m_DepthAttachment = 0;
		FramebufferProperties m_Properties;

		std::vector<FramebufferTextureSpecification> m_ColorAttachmentSpecs;
		FramebufferTextureSpecification m_DepthAttachmentSpec;

		std::vector<uint32_t> m_ColorAttachments;
		uint32_t m_DepthAttachment = 0;
	};


}