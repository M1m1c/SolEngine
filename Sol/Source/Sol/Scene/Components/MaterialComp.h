#pragma once

#include "GalaxyDraw/Interfaces/IMaterial.h"
#include "GalaxyDraw/Interfaces/MaterialData.h"
#include <glm/glm.hpp>


namespace Sol
{
	//TODO make this use and load textures, also every entity with a ModelComp should probably have a material comp
	struct MaterialComp
	{
		glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };

		//std::string TexturePath;

		MaterialComp() = default;
		
		MaterialComp(const MaterialComp&) = default;

		MaterialComp(const std::string& texturePath, EntityID entityID)
		{
			m_MaterialIndex = GalaxyDraw::IMaterial::Create(texturePath,entityID);
		}

		uint32_t GetMaterialIndex() { return m_MaterialIndex; }

		/*glm::vec4& GetMaterialColor() 
		{ 
			auto mat = GalaxyDraw::IMaterial::GetMaterial(m_MaterialIndex);
			return mat->Color;
		}*/

		/*uint32_t& GetMaterialTextureIndex()
		{
			auto mat = GalaxyDraw::IMaterial::GetMaterial(m_MaterialIndex);
			return mat->TextureIndex;
		}*/

		std::string& GetMaterialName()
		{
			auto mat = GalaxyDraw::IMaterial::GetMaterial(m_MaterialIndex);
			SOL_CORE_ASSERT(mat, "No material found!");
			return mat->Name;
		}

		std::string& GetTexturePath()
		{
			auto mat = GalaxyDraw::IMaterial::GetMaterial(m_MaterialIndex);
			SOL_CORE_ASSERT(mat, "No material found!");
			return mat->DiffuseTexturePath;
		}

		//TODO add set material function that checks with material manager if the material index is valid
		//TODO add CreateNewMaterialInstance function that asks material manager to create a new material and set it to this
	private:
		uint32_t m_MaterialIndex = 0;

	};
}