#include "solpch.h"

#include "OpenGL_Texture.h"
#include "GLMacros.h"

#include "GalaxyDraw/Interfaces/Shader.h"

#include <glad/glad.h>
#include <stb_image.h>

namespace GalaxyDraw
{
	OpenGL_Texture2D::OpenGL_Texture2D(const std::string& path) : m_Path(path)
	{
		int widthImg, heightImg, numColCh;
		stbi_set_flip_vertically_on_load(true);
		stbi_uc* data = stbi_load(path.c_str(), &widthImg, &heightImg, &numColCh, 0);

		SOL_CORE_ASSERT(data, "Failed to Load image!");
		m_Width = widthImg;
		m_Height = heightImg;

		GLenum internalFormat = 0, dataFormat = 0;
		if (numColCh == 4)
		{
			internalFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
		}
		else if (numColCh == 3)
		{
			internalFormat = GL_RGB8;
			dataFormat = GL_RGB;
		}

		SOL_CORE_ASSERT(internalFormat && dataFormat, "Format not supported!");

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, internalFormat, m_Width, m_Height);

		glTexParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);
	}

	OpenGL_Texture2D::OpenGL_Texture2D(const char* image, uint32_t texType, uint32_t slot, uint32_t format, uint32_t pixelType)
	{
		m_Path = image;
		// Assigns the type of the texture ot the texture object
		type = texType;

		// Stores the width, height, and the number of color channels of the image
		int widthImg, heightImg, numColCh;
		// Flips the image so it appears right side up
		stbi_set_flip_vertically_on_load(true);
		// Reads the image from a file and stores it in bytes
		unsigned char* bytes = stbi_load(image, &widthImg, &heightImg, &numColCh, 0);

		SOL_CORE_ASSERT(bytes, "Failed to Load image!");
		m_Width = widthImg;
		m_Height = heightImg;

		// Generates an OpenGL texture object
		GLCall(glGenTextures(1, &m_RendererID));
		// Assigns the texture to a Texture Unit
		GLCall(glActiveTexture(GL_TEXTURE0 + slot));
		unit = slot;
		GLCall(glBindTexture(texType, m_RendererID));

		// Configures the type of algorithm that is used to make the image smaller or bigger
		GLCall(glTexParameteri(texType, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
		GLCall(glTexParameteri(texType, GL_TEXTURE_MAG_FILTER, GL_NEAREST));

		// Configures the way the texture repeats (if it does at all)
		GLCall(glTexParameteri(texType, GL_TEXTURE_WRAP_S, GL_REPEAT));
		GLCall(glTexParameteri(texType, GL_TEXTURE_WRAP_T, GL_REPEAT));

		// Extra lines in case you choose to use GL_CLAMP_TO_BORDER
		// float flatColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
		// glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, flatColor);

		// Assigns the image to the OpenGL Texture object
		GLCall(glTexImage2D(texType, 0, GL_RGBA, widthImg, heightImg, 0, format, pixelType, bytes));
		// Generates MipMaps
		GLCall(glGenerateMipmap(texType));

		// Deletes the image data as it is already in the OpenGL Texture object
		stbi_image_free(bytes);

		// Unbinds the OpenGL Texture object so that it can't accidentally be modified
		GLCall(glBindTexture(texType, 0));
	}

	OpenGL_Texture2D::~OpenGL_Texture2D()
	{
		glDeleteTextures(1, &m_RendererID);
	}

	void OpenGL_Texture2D::texUnit(Shader& shader, const char* uniform, uint32_t _unit)
	{
		// Gets the location of the uniform
		GLCall(GLuint texUni = glGetUniformLocation(shader.GetID(), uniform));
		// Shader needs to be activated before changing the value of a uniform
		shader.Bind();
		// Sets the value of the uniform
		GLCall(glUniform1i(texUni, _unit));
	}

	void OpenGL_Texture2D::Bind(uint32_t slot) const
	{
		glBindTextureUnit(slot, m_RendererID);
		/*	GLCall(glActiveTexture(GL_TEXTURE0 + unit));
			GLCall(glBindTexture(type, m_RendererID));*/
	}

	void OpenGL_Texture2D::Unbind() const
	{
		GLCall(glBindTexture(type, 0));
	}

	void OpenGL_Texture2D::Delete()
	{
		GLCall(glDeleteTextures(1, &m_RendererID));
	}
}