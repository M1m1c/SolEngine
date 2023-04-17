#pragma once
#include "Material.h"
#include "Texture.h"
#include "GalaxyDraw/Interfaces/TextureArray.h"
#include "Sol/Core/KeyedVector.h"
#include <vector>

namespace GalaxyDraw
{

	static class MaterialManager
	{
	public:

		static uint32_t SetupMaterial(const std::string& texturePath, bool shouldCreateNewMaterial = false );	
		static std::shared_ptr<Material> GetMaterial(uint32_t index);
		static void DiscardMaterial(uint32_t index);

		static MaterialManager& GetInstance()
		{
			static MaterialManager instance;
			return instance;
		}

		MaterialManager(const MaterialManager&) = delete;
		MaterialManager& operator=(const MaterialManager&) = delete;

	private:
		MaterialManager();
		static uint32_t CreateNewTexture(const std::string& texturePath);
		static uint32_t CreateNewMaterial(const uint32_t& textureIndex);
	private:
		std::vector <std::shared_ptr<Material>> m_Materials{nullptr};
		//TODO instead of this holding loaded textures it will hold a texture array which we can access the loaded textures from
		//TODO this needs to keep track of how many materials are using the texture, if the materials using a texture is 0, it shoudl call remocve on the texture array
		//KeyedVector <std::string, std::shared_ptr<Texture>> m_LoadedTextures;
		std::shared_ptr<TextureArray> m_TextureArray;
		std::map<std::string, std::vector<uint32_t>> m_TextureToMaterialsMap;
	};


}