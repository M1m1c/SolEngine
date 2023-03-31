#pragma once
#include "Material.h"
#include "Texture.h"
#include <vector>
#include "Sol/Core/KeyedVector.h"

namespace GalaxyDraw
{

	static class MaterialManager
	{
	public:

		static uint32_t SetupMaterial(const std::string& texturePath);
		static uint32_t CreateNewMaterial(const std::string& texturePath);
		static std::shared_ptr <Material> GetMaterial(uint32_t index);
		static void DiscardMaterial(uint32_t index);

		static MaterialManager& GetInstance()
		{
			static MaterialManager instance;
			return instance;
		}

		MaterialManager(const MaterialManager&) = delete;
		MaterialManager& operator=(const MaterialManager&) = delete;

	private:
		MaterialManager();
	private:
		std::vector < std::shared_ptr<Material>> m_Materials;
		KeyedVector <std::string, std::shared_ptr<Texture>> m_LoadedTextures;
	};


}