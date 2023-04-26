#include "solpch.h"
#include "IMaterial.h"
#include "TextureManager.h"
#include "MaterialData.h"
#include "Renderer3D.h"

namespace GalaxyDraw {

	uint32_t IMaterial::CreateNew(const std::string& texturePath, const EntityID entityID)
	{
		return Renderer3D::CreateNewMaterial(texturePath, entityID);
	}

	uint32_t IMaterial::UpdateExisting(const std::string& texturePath, const uint32_t matIndex, const EntityID entityID)
	{
		return Renderer3D::UpdateExistingMaterial(texturePath,matIndex, entityID);
	}

	uint32_t IMaterial::SwapMaterial(const uint32_t matIndex, const EntityID entityID)
	{
		return Renderer3D::SwapMaterial(matIndex, entityID);
	}

	uint32_t IMaterial::GetDefaultMaterialIndex()
	{
		return 0;
	}

	std::shared_ptr<MaterialData> IMaterial::GetMaterial(uint32_t materialIndex)
	{
		return Renderer3D::GetMaterial(materialIndex);
	}

	
}