#pragma once
#include "Material.h"
#include "Texture.h"
#include "GalaxyDraw/Interfaces/TextureArray.h"

#include <vector>
#include <map>

namespace GalaxyDraw
{

	static class MaterialManager
	{
	public:
		static void Initialize(uint32_t width=2, uint32_t height=2,uint32_t maxTextures = 100, uint32_t textureUnit = 0);
		static uint32_t SetupMaterial(const std::string& texturePath, bool shouldCreateNewMaterial = false);
		static uint32_t GetDefaultMaterial();
		static std::shared_ptr<Material> GetMaterial(uint32_t materialIndex);
		static const std::string GetTexturePath(uint32_t textureIndex);
		static void DiscardMaterial(uint32_t index);

		static void BindTextureArray();
		static uint32_t GetTextureUnit();

		static MaterialManager& GetInstance()
		{
			static MaterialManager instance;
			return instance;
		}

		MaterialManager(const MaterialManager&) = delete;
		MaterialManager& operator=(const MaterialManager&) = delete;

	private:
		MaterialManager();
		static int CreateNewTexture(const std::string& texturePath);
		static uint32_t CreateNewMaterial(const uint32_t& textureIndex, std::string matName ="");
	private:
		std::vector <std::shared_ptr<Material>> m_Materials;
		//TODO instead of this holding loaded textures it will hold a texture array which we can access the loaded textures from
		//TODO this needs to keep track of how many materials are using the texture, if the materials using a texture is 0, it shoudl call remocve on the texture array
		//KeyedVector <std::string, std::shared_ptr<Texture>> m_LoadedTextures;
		std::shared_ptr<TextureArray> m_TextureArray;
		std::map<std::string, std::vector<uint32_t>> m_TextureToMaterialsMap;
		std::map<uint32_t, std::string> m_TextureIndexToTexturePathMap;
		uint32_t m_DefaultMaterialIndex;
	};


}