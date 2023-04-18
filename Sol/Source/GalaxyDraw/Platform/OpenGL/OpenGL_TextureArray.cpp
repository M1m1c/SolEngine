#include "solpch.h"
#include "OpenGL_TextureArray.h"
#include <glad/glad.h>
#include <stb_image.h>

namespace GalaxyDraw {
	OpenGL_TextureArray::OpenGL_TextureArray(uint32_t width, uint32_t height,uint32_t maxTextures, uint32_t textureUnit) 
		: m_Width(width), m_Height(height), m_MaxTextures(maxTextures), m_TextureUnit(textureUnit)
	{
		glGenTextures(1, &m_RendererID);
		glBindTexture(GL_TEXTURE_2D_ARRAY, m_RendererID);
		glBindTextureUnit(m_TextureUnit, m_RendererID);

		// Allocate storage for the texture array
		glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGBA8, width, height, maxTextures);

		// Set texture parameters
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glBindTexture(GL_TEXTURE_2D_ARRAY, 0);

		m_defaultTextureIndex = CreateDefaultTexture();
	}

	OpenGL_TextureArray::~OpenGL_TextureArray()
	{
		glDeleteTextures(1, &m_RendererID);
	}

	int OpenGL_TextureArray::AddTexture(const std::string& path)
	{
		if (m_NumTextures + 1 > m_MaxTextures) 
		{
			SOL_CORE_ASSERT(false, "Out of range! Attempting to add more textures than provided maximum. Increase amount allowed when constructing TexureArray.");
			return -1; 
		}

		int widthImg, heightImg, numColCh;
		stbi_set_flip_vertically_on_load(true);
		stbi_uc* data = stbi_load(path.c_str(), &widthImg, &heightImg, &numColCh, 0);
		//TODO if it can't load the image we should use a missing texutre magenta to show that it is missing

		SOL_CORE_ASSERT(data, "Failed to Load image!");

		bool reUsedIndex = false;
		uint32_t textureIndex = GetAvailableTextureIndex(reUsedIndex);

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

	uint32_t OpenGL_TextureArray::CreateDefaultTexture()
	{
		
		bool reUsedIndex = false;
		uint32_t textureIndex = 0;

		// Create a white texture of the specified size
		uint32_t whiteColor = 0Xffffffff;
		glBindTexture(GL_TEXTURE_2D_ARRAY, m_RendererID);
		glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, textureIndex, m_Width, m_Height, 1, GL_RGBA, GL_UNSIGNED_BYTE, &whiteColor);

		m_NumTextures++;

		if (!reUsedIndex) { m_NextUsableIndex = m_NumTextures; }

		//glBindTexture(GL_TEXTURE_2D_ARRAY, 0);

		return textureIndex;
	}

	uint32_t OpenGL_TextureArray::GetAvailableTextureIndex(bool& reUsedIndex)
	{
		uint32_t textureIndex = 0;
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

	int OpenGL_TextureArray::GetTextureIndex(const std::string& path)
	{
		SOL_CORE_ASSERT(IsTextureLoaded(path), "Texture is not loaded! Can't access its index.");
		return m_LoadedTextures.Get(path);
	}

	//TODO do we actually need to do any of this?
	void OpenGL_TextureArray::Bind() const
	{
		//glActiveTexture(m_TextureUnit);
		//glBindTexture(GL_TEXTURE_2D_ARRAY, m_RendererID);
		//glBindTextureUnit(m_TextureUnit, m_RendererID);
		/*glActiveTexture(textureUnit);
		glBindTexture(GL_TEXTURE_2D_ARRAY, m_RendererID);*/
	}

	bool OpenGL_TextureArray::operator==(const TextureArray& other) const
	{
		return false;
	}
}
