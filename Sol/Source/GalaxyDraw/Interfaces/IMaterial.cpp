#include "solpch.h"
#include "IMaterial.h"
#include "TextureManager.h"
#include "MaterialData.h"
#include "Renderer3D.h"

namespace GalaxyDraw {

	uint32_t IMaterial::Create(const std::string& texturePath, const EntityID entity)
	{
		return Renderer3D::SetupMaterial(texturePath, entity);
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