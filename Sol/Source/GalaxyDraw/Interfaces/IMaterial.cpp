#include "solpch.h"
#include "IMaterial.h"

namespace GalaxyDraw {

	uint32_t IMaterial::Create(const std::string& texturePath, const EntityID entity)
	{
		return 0;
	}

	uint32_t IMaterial::GetDefaultMaterialIndex()
	{
		return 0;
	}

	/*std::shared_ptr<Material> IMaterial::GetMaterial(uint32_t materialIndex)
	{
		return MaterialManager::GetMaterial(materialIndex);
	}*/
}