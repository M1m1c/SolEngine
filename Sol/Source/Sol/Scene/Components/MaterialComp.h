#pragma once

//#include "GalaxyDraw/Interfaces/MaterialManager.h"
#include "GalaxyDraw/Interfaces/Texture.h"
#include <glm/glm.hpp>


namespace Sol
{
	//TODO make this use and load textures, also every entity with a ModelComp should probably have a material comp
	struct MaterialComp
	{
		glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };

		std::string TexturePath;

		MaterialComp() = default;
		MaterialComp(const MaterialComp&) = default;
		MaterialComp(const std::string& texturePath) : TexturePath(texturePath) {
			//TODO ok including material manager causes this to crash, 
			// so do something similar to model comp and call an interface that can solve thsi for us
			//m_MaterialIndex = GalaxyDraw::MaterialManager::SetupMaterial(texturePath);
		}

		uint32_t GetMaterialIndex() { return m_MaterialIndex; }
		//TODO add set material function that checks with material manager if the material index is valid
		//TODO add CreateNewMaterialInstance function that asks material manager to create a new material and set it to this
	private:
		uint32_t m_MaterialIndex;

	};
}