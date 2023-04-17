#pragma once

#include "GalaxyDraw/Interfaces/IMaterial.h"
#include "GalaxyDraw/Interfaces/Texture.h"
#include <glm/glm.hpp>


namespace Sol
{
	//TODO make this use and load textures, also every entity with a ModelComp should probably have a material comp
	struct MaterialComp
	{
		//TODO this should only need to hold a material index, whihc we can then use to read from
		glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };

		//std::string TexturePath;

		MaterialComp() 
		{
			m_MaterialIndex = GalaxyDraw::IMaterial::GetDefaultMaterial();
		}

		MaterialComp(const MaterialComp&) = default;

		MaterialComp(const std::string& texturePath) 
		{
			m_MaterialIndex = GalaxyDraw::IMaterial::Create(texturePath);
		}

		uint32_t GetMaterialIndex() { return m_MaterialIndex; }
		//TODO add set material function that checks with material manager if the material index is valid
		//TODO add CreateNewMaterialInstance function that asks material manager to create a new material and set it to this
	private:
		uint32_t m_MaterialIndex = 0;

	};
}