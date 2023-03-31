#pragma once
#include "Material.h"
#include "Texture.h"
#include <vector>

namespace GalaxyDraw
{

	static class MaterialManager
	{
	public:

		static uint32_t CreateMaterial(const std::string& path);
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

		static uint32_t CreateTexture(const std::string& path);
	private:
		std::vector < std::shared_ptr<Material>> m_Materials;
		std::vector < std::shared_ptr<Texture>> m_LoadedTextures;
	};


}