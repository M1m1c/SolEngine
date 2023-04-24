#pragma once

#include "Sol/SolDefines.h"
#include <string>

namespace GalaxyDraw
{
	struct MaterialData;
	class IMaterial
	{
	public:
		static uint32_t CreateNew(const std::string& texturePath, const EntityID entityID);
		static uint32_t UpdateExisting(const std::string& texturePath, const uint32_t matIndex, const EntityID entityID);
		static uint32_t SwapMaterial(const uint32_t matIndex, const EntityID entityID);
		static uint32_t GetDefaultMaterialIndex();
		static std::shared_ptr<MaterialData> GetMaterial(uint32_t materialIndex);
	};

}