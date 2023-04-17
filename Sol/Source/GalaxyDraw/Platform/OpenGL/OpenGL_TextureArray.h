#pragma once
#include "GalaxyDraw/Interfaces/TextureArray.h"
#include "Sol/Core/KeyedVector.h"
#include <queue>

namespace GalaxyDraw {

	class OpenGL_TextureArray : public TextureArray
	{
	public:
		OpenGL_TextureArray(uint32_t maxTextures);
		~OpenGL_TextureArray();

		// Inherited via TextureArray
		virtual int AddTexture(const std::string& path) override;

		virtual void RemoveTexture(const std::string& path) override;

		virtual bool IsTextureLoaded(const std::string& path) override;

		virtual int GetTextureIndex(const std::string& path) override;

		virtual uint32_t GetRendererID() const override;

		virtual uint32_t Size() const override { return m_NumTextures; };

		virtual void Bind(uint32_t textureUnit = 0) const override;

		virtual bool operator==(const TextureArray& other) const override;

	private:
		uint32_t m_RendererID; // The ID of the texture array in GPU memory
		uint32_t m_MaxTextures = 0;
		uint32_t m_NumTextures = 0;
		uint32_t m_NextUsableIndex = 0;
		KeyedVector <std::string, uint32_t> m_LoadedTextures;
		//TODO this queue is to be used for reusing indices of freed textures, 
		// when a texture is removed its index is added to it.
		// when a new texture is added it will deque an index if there are any avilable, otherwise it will get the m_NumTextures
		std::queue<uint32_t> m_FreeIndices;
	};
	
}