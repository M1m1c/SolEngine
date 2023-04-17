#pragma once
#include "Material.h"
#include <string>

namespace GalaxyDraw
{
	class IMaterial
	{
	public:
		static uint32_t Create(const std::string& texturePath);
		static uint32_t GetDefaultMaterial();
		static std::shared_ptr<Material> GetMaterial(uint32_t materialIndex);
	};

}