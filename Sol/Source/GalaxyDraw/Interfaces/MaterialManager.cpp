#include "solpch.h"
#include "MaterialManager.h"

namespace GalaxyDraw {

	MaterialManager::MaterialManager()
	{
		//TODO this is temporary and needs to be set elsewhere
		uint32_t maxTextures = 100;
		m_TextureArray = TextureArray::Create(maxTextures);
	}

	uint32_t MaterialManager::SetupMaterial(const std::string& texturePath, bool shouldCreateNewMaterial)
	{
		//index 0 is an invalid material
		if (texturePath == "") { return 0; }
		uint32_t materialIndex = 0;

		auto& s = MaterialManager::GetInstance();
		bool isTextureLoaded = s.m_TextureArray->IsTextureLoaded(texturePath);

		if (isTextureLoaded && shouldCreateNewMaterial)
		{
			//Creates a new material using the texture that is already loaded
			uint32_t texIndex = s.m_TextureArray->GetTextureIndex(texturePath);

			materialIndex = CreateNewMaterial(texIndex);

			std::vector<uint32_t>& matIndices = s.m_TextureToMaterialsMap[texturePath];
			matIndices.push_back(materialIndex);
		}
		else if (isTextureLoaded)
		{
			//Gets the first material that uses the texture
			auto it = s.m_TextureToMaterialsMap.find(texturePath);
			if (it != s.m_TextureToMaterialsMap.end())
			{
				std::vector<uint32_t>& matIndices = it->second;
				materialIndex = matIndices.size() > 0 ? matIndices[0] : 0;
			}
		}
		else
		{
			//loads the texture and creates a material that uses it.
			uint32_t texIndex = CreateNewTexture(texturePath);
			materialIndex = CreateNewMaterial(texIndex);
			s.m_TextureToMaterialsMap.insert(std::make_pair(texturePath, std::vector<uint32_t>({ materialIndex })));

		}

		return materialIndex;
	}

	std::shared_ptr<Material> MaterialManager::GetMaterial(uint32_t index)
	{
		auto& s = MaterialManager::GetInstance();
		if (index >= s.m_Materials.size()) { return nullptr; }
		return s.m_Materials[index];
	}

	uint32_t MaterialManager::CreateNewTexture(const std::string& texturePath)
	{
		auto& s = MaterialManager::GetInstance();
		
		uint32_t textureIndex = s.m_TextureArray->AddTexture(texturePath);

		return textureIndex;
	}

	uint32_t MaterialManager::CreateNewMaterial(const uint32_t& textureIndex)
	{
		auto& s = MaterialManager::GetInstance();
		//TODO we might run into trouble here since the first material index might be 0 if the size is 0
		uint32_t materialIndex = s.m_Materials.size();
		auto mat = std::make_shared<Material>(textureIndex);
		s.m_Materials.push_back(mat);

		return materialIndex;
	}

}
