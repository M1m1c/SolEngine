#pragma once
#include <string>

namespace GalaxyDraw
{
	class IMaterial
	{
	public:
		static uint32_t Create(const std::string& texturePath);
		static uint32_t GetDefaultMaterial();
	};

}