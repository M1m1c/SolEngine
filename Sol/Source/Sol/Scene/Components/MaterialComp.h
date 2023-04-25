#pragma once

#include "GalaxyDraw/Interfaces/IMaterial.h"
#include "GalaxyDraw/Interfaces/MaterialData.h"
#include <glm/glm.hpp>


namespace Sol
{
	struct InstancedMaterialProps
	{
		glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };
	};

	struct MaterialComp
	{
		
		InstancedMaterialProps Properties;

		MaterialComp() = default;
		
		MaterialComp(const MaterialComp&) = default;

		//Creates a new material for this component to use
		MaterialComp(const std::string& texturePath, EntityID entityID)
		{
			m_MaterialIndex = GalaxyDraw::IMaterial::CreateNew(texturePath,entityID);
		}

		//Updates existing material on component and for all instances using it
		MaterialComp(const std::string& texturePath, const uint32_t matIndex, const EntityID entityID)
		{
			if(matIndex==0)
			{
				m_MaterialIndex = 0;
				return;
			}
			m_MaterialIndex = GalaxyDraw::IMaterial::UpdateExisting(texturePath,matIndex, entityID);
		}

		//Swaps this components material for antother
		MaterialComp(const uint32_t matIndex, const EntityID entityID)
		{
			SwapMaterial(matIndex, entityID);
		}

		uint32_t GetMaterialIndex() { return m_MaterialIndex; }


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

		void SwapMaterial(const uint32_t matIndex, const EntityID entityID) 
		{
			m_MaterialIndex = GalaxyDraw::IMaterial::SwapMaterial(matIndex, entityID);
		}

	private:
		uint32_t m_MaterialIndex = 0;

	};
}