#include "solpch.h"
#include "TextureManager.h"
#include "Texture.h"



namespace GalaxyDraw 
{

	void TextureManager::Initialize()
	{
		auto defaultTexture = Texture2D::Create(1, 1);
		unsigned char whiteColor[] = { 255, 255, 255, 255 };
		defaultTexture->SetData(&whiteColor, sizeof(uint32_t));

		auto& s = TextureManager::GetInstance();
		s.m_LoadedTextures.push_back("", { defaultTexture,1 });
	}

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

	bool TextureManager::IsTextureLoaded(const std::string& path)
	{
		auto& s = TextureManager::GetInstance();
		if (s.m_LoadedTextures.size() == 0) { return false; }
		return s.m_LoadedTextures.Exists(path);
	}

	std::shared_ptr<Texture> TextureManager::GetTexture(std::string filePath)
	{
		auto& s = TextureManager::GetInstance();
		if (s.m_LoadedTextures.Exists(filePath))
		{
			return s.m_LoadedTextures.Get(filePath).first;
		}
		return nullptr;
	}

	void TextureManager::DiscardTextureInstance(std::string filePath)
	{
		auto& s = TextureManager::GetInstance();
		if (s.m_LoadedTextures.Exists(filePath))
		{
			auto& textureInstance = s.m_LoadedTextures.Get(filePath);
			textureInstance.second--;
			
			if (textureInstance.second == 0 && filePath!="")
			{
				//TODO removeclear up that index in m_loaded textures
				s.m_LoadedTextures.eraseWithKey(filePath);
			}
		}
	}
}
