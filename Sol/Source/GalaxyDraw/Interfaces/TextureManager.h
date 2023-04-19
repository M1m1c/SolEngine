#pragma once
#include "Sol/Core/KeyedVector.h"

#include <string>
#include <memory>



namespace GalaxyDraw 
{
	class Texture;

	//TODO this class will essentially replace the material manager.
	// it is supposed to be able to load and hold texture, giving us access to tehm via indexes/ keys
	static class TextureManager
	{
	public:
		
		static void Initialize();

		static TextureManager& GetInstance()
		{
			static TextureManager instance;
			return instance;
		}

		~TextureManager() = default;

		//TODO if textre is already loaded give then use that one
		static std::shared_ptr<Texture> LoadTexture(const std::string& filePath);
		static std::shared_ptr<Texture> GetTexture(std::string filePath);
		//TODO reduce using count, if there are no more then unload the texture
		static void DiscardTextureInstance(std::string filePath);

	private:
		TextureManager();
	private:

		//key is FilePath, <Texture, usingCount>
		KeyedVector <std::string,std::pair<std::shared_ptr<Texture>, int32_t>> m_LoadedTextures;
		//std::map<uint32_t, std::string> m_TextureIndexToTexturePathMap;
	};
}