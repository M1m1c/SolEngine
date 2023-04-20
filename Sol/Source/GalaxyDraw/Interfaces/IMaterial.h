#pragma once

#include "Sol/SolDefines.h"
#include <string>

namespace GalaxyDraw
{
	class IMaterial
	{
	public:
		static uint32_t Create(const std::string& texturePath, const EntityID entity);
		static uint32_t GetDefaultMaterialIndex();
		//static std::shared_ptr<Material> GetMaterial(uint32_t materialIndex);
	};

}