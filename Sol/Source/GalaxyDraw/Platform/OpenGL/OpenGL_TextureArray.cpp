#include "solpch.h"
#include "OpenGL_TextureArray.h"
#include <glad/glad.h>
#include <stb_image.h>

namespace GalaxyDraw {
	OpenGL_TextureArray::OpenGL_TextureArray(uint32_t maxTextures)
	{
		glGenTextures(1, &m_RendererID);
		glBindTexture(GL_TEXTURE_2D_ARRAY, m_RendererID);

		// Allocate storage for the texture array
		glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGBA8, 0, 0, maxTextures);

		// Set texture parameters
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
	}

	OpenGL_TextureArray::~OpenGL_TextureArray()
	{
		glDeleteTextures(1, &m_RendererID);
	}

	uint32_t OpenGL_TextureArray::AddTexture(const std::string& path)
	{
		int widthImg, heightImg, numColCh;
		stbi_set_flip_vertically_on_load(true);
		stbi_uc* data = stbi_load(path.c_str(), &widthImg, &heightImg, &numColCh, 0);

		SOL_CORE_ASSERT(data, "Failed to Load image!");

		uint32_t textureIndex = 0;
		bool reUsedIndex = false;
		if (!m_FreeIndices.empty())
		{
			textureIndex = m_FreeIndices.front();
			m_FreeIndices.pop();
			reUsedIndex = true;
		}
		else
		{
			textureIndex = m_NextUsableIndex;
		}

		m_LoadedTextures.push_back(path, textureIndex);

		// Bind the texture array
		glBindTexture(GL_TEXTURE_2D_ARRAY, m_RendererID);

		// Add the texture data to the texture array
		glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, textureIndex, widthImg, heightImg, 1, GL_RGBA, GL_UNSIGNED_BYTE, data);

		// Update the number of textures in the array
		m_NumTextures++;

		if (!reUsedIndex) { m_NextUsableIndex = m_NumTextures; }

		// Free the texture data
		stbi_image_free(data);

		// Unbind the texture array
		glBindTexture(GL_TEXTURE_2D_ARRAY, 0);

		return textureIndex;
	}

	void OpenGL_TextureArray::RemoveTexture(const std::string& filePath)
	{
		
		if (IsTextureLoaded(filePath))
		{
			uint32_t textureIndex = m_LoadedTextures.Get(filePath);

			// Mark the index as free
			m_FreeIndices.push(textureIndex);

			// Bind the texture array
			glBindTexture(GL_TEXTURE_2D_ARRAY, m_RendererID);

			// Clear the texture data at the index
			glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, textureIndex, 0, 0, 1, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

			// Remove the entry from the loaded textures container
			m_LoadedTextures.eraseWithKey(filePath);

			// Update the number of textures in the array
			m_NumTextures--;

			// Unbind the texture array
			glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
		}
	}

	bool OpenGL_TextureArray::IsTextureLoaded(const std::string& path)
	{
		if (m_LoadedTextures.size() == 0) { return false; }
		return m_LoadedTextures.Exists(path);
	}

	uint32_t OpenGL_TextureArray::GetTextureIndex(const std::string& path)
	{
		SOL_CORE_ASSERT(IsTextureLoaded(path), "Texture is not loaded! Can't access its index.");
		return m_LoadedTextures.Get(path);
	}

	uint32_t OpenGL_TextureArray::GetRendererID() const
	{
		return m_RendererID;
	}

	void OpenGL_TextureArray::Bind(uint32_t textureUnit) const
	{
		glBindTextureUnit(textureUnit, m_RendererID);
		/*glActiveTexture(textureUnit);
		glBindTexture(GL_TEXTURE_2D_ARRAY, m_RendererID);*/
	}

	bool OpenGL_TextureArray::operator==(const TextureArray& other) const
	{
		return false;
	}
}
