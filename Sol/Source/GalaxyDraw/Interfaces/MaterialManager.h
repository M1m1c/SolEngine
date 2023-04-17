#pragma once
#include "Material.h"
#include "Texture.h"
#include <vector>
#include "Sol/Core/KeyedVector.h"

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
		//TODO instead of thsi holding loaded textures it will hold a texture array which we can access the loaded textures from
		KeyedVector <std::string, std::shared_ptr<Texture>> m_LoadedTextures;
		std::map<std::string, std::vector<uint32_t>> m_TextureToMaterialsMap;
	};


}