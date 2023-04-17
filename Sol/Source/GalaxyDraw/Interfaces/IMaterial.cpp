#include "solpch.h"
#include "IMaterial.h"

#include "MaterialManager.h"

uint32_t GalaxyDraw::IMaterial::Create(const std::string& texturePath)
{
    return GalaxyDraw::MaterialManager::SetupMaterial(texturePath);
}

uint32_t GalaxyDraw::IMaterial::GetDefaultMaterial()
{
    return GalaxyDraw::MaterialManager::GetDefaultMaterial();
}
