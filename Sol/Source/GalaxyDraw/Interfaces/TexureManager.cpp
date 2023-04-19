#include "solpch.h"
#include "TexureManager.h"

namespace GalaxyDraw {

	std::shared_ptr<Texture> TextureManager::LoadTexture(const std::string& filePath)
	{
		auto& s = TextureManager::GetInstance();
		if (s.m_LoadedTextures.Exists(filePath))
		{
			auto& textureInstance = s.m_LoadedTextures.Get(filePath);
			textureInstance.second++;
			return textureInstance.first;
		}

		auto& newTexture = Texture2D::Create(filePath);

		s.m_LoadedTextures.push_back(filePath, { newTexture, 1 });
		return newTexture;
	}

	std::shared_ptr<Texture> TextureManager::GetTexture(std::string filePath)
	{
		auto& s = TextureManager::GetInstance();
		
		return s.m_LoadedTextures.Get(filePath).first;
		
	}

	void TextureManager::DiscardTexture(std::string filePath)
	{
		auto& s = TextureManager::GetInstance();
		if (s.m_LoadedTextures.Exists(filePath))
		{
			auto& textureInstance = s.m_LoadedTextures.Get(filePath);
			textureInstance.second--;
			
			if (textureInstance.second == 0)
			{
				//TODO removeclear up that index in m_loaded textures
				s.m_LoadedTextures.eraseWithKey(filePath);
			}
		}
	}
}
