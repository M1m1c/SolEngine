#include "solpch.h"
#include "MaterialManager.h"

namespace GalaxyDraw {

	MaterialManager::MaterialManager()
	{
	}

	uint32_t MaterialManager::SetupMaterial(const std::string& texturePath)
	{
		//if there already is a material with this texture then give us that ones index,
		//otherwise create a new material
		return uint32_t();
	}

	uint32_t MaterialManager::CreateNewMaterial(const std::string& texturePath)
	{
		auto& s = MaterialManager::GetInstance();
		uint32_t materialIndex = s.m_Materials.size();
		uint32_t textureIndex = s.m_LoadedTextures.size();

		auto texture = Texture2D::Create(texturePath);
		s.m_LoadedTextures.push_back(texturePath,texture);

		auto mat = std::make_shared<Material>(textureIndex);
		s.m_Materials.push_back(mat);
		
		//TODO load texture
		// store texture index
		// create and store new material with texture indices

		return materialIndex;
	}
	
}
