#include "solpch.h"
#include "IMaterial.h"
#include "MaterialManager.h"

namespace GalaxyDraw {

	uint32_t IMaterial::Create(const std::string& texturePath)
	{
		return MaterialManager::SetupMaterial(texturePath);
	}

	uint32_t IMaterial::GetDefaultMaterial()
	{
		return MaterialManager::GetDefaultMaterial();
	}

	std::shared_ptr<Material> IMaterial::GetMaterial(uint32_t materialIndex)
	{
		return MaterialManager::GetMaterial(materialIndex);
	}
}
