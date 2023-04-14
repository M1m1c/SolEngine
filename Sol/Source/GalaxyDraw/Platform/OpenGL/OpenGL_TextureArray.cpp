#include "solpch.h"
#include "OpenGL_TextureArray.h"
#include <glad/glad.h>

namespace GalaxyDraw {
	OpenGL_TextureArray::OpenGL_TextureArray(uint32_t maxTextures)
	{
		glGenTextures(1, &m_RendererID);
		glBindTexture(GL_TEXTURE_2D_ARRAY, m_RendererID);
		glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA, 0, 0, maxTextures, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	}
	OpenGL_TextureArray::~OpenGL_TextureArray()
	{
		glDeleteTextures(1, &m_RendererID);
	}
	void OpenGL_TextureArray::addTexture(Sol::s_ptr<Texture> texture)
	{
		//texture->GetRendererID()
		auto width = texture->GetWidth();
		auto height = texture->GetHeight();
		glBindTexture(GL_TEXTURE_2D_ARRAY, m_RendererID);
		//glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, index, width, height, 1, GL_RGBA, GL_UNSIGNED_BYTE, data);
		
	}

	uint32_t OpenGL_TextureArray::GetRendererID() const
	{
		return m_RendererID;
	}

	void OpenGL_TextureArray::Bind(uint32_t textureUnit) const
	{
		glActiveTexture(textureUnit);
		glBindTexture(GL_TEXTURE_2D_ARRAY, m_RendererID);
	}

	bool OpenGL_TextureArray::operator==(const TextureArray& other) const
	{
		return false;
	}
}
