#include "solpch.h"
#include "OpenGL_Framebuffer.h"

#include <glad/glad.h>

namespace GalaxyDraw
{
	OpenGL_Framebuffer::OpenGL_Framebuffer(const FramebufferProperties& properties) :
		m_Properties(properties)
	{
		Invalidate();
	}

	OpenGL_Framebuffer::~OpenGL_Framebuffer()
	{
		glDeleteBuffers(1, &m_RendererID);
		glDeleteTextures(1, &m_ColorAttachment);
		glDeleteTextures(1, &m_DepthAttachment);
	}

	void OpenGL_Framebuffer::Invalidate()
	{
		if (m_RendererID)
		{
			glDeleteTextures(1, &m_ColorAttachment);
			glDeleteTextures(1, &m_DepthAttachment);
		}

		glCreateFramebuffers(1, &m_RendererID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

		glCreateTextures(GL_TEXTURE_2D, 1, &m_ColorAttachment);
		glBindTexture(GL_TEXTURE_2D, m_ColorAttachment);

		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RGB8,
			m_Properties.Width,
			m_Properties.Height,
			0,
			GL_RGBA,
			GL_UNSIGNED_BYTE,
			nullptr);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorAttachment, 0);

		glCreateTextures(GL_TEXTURE_2D, 1, &m_DepthAttachment);
		glBindTexture(GL_TEXTURE_2D, m_DepthAttachment);
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, m_Properties.Width, m_Properties.Height);
		/*glTexImage2D(
			GL_TEXTURE_2D,
			0, 
			GL_DEPTH24_STENCIL8,
			m_Properties.Width,
			m_Properties.Height,
			0,
			GL_DEPTH_STENCIL,
			GL_UNSIGNED_INT_24_8,
			NULL);*/
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_DepthAttachment, 0);

		SOL_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE,"Framebuffer is incomplete!");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGL_Framebuffer::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
		glViewport(0, 0, m_Properties.Width, m_Properties.Height);
	}

	void OpenGL_Framebuffer::UnBind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGL_Framebuffer::Resize(uint32_t width, uint32_t height)
	{
		m_Properties.Width = width;
		m_Properties.Height = height;
		Invalidate();
	}
}
