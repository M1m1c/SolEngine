#pragma once

#include <GalaxyDraw/Interfaces/IModel.h>
#include <GalaxyDraw/Interfaces/Texture.h>
#include "Sol/SolDefines.h"

namespace Sol {
	struct ModelComp
	{
		std::string ModelPath;
		std::string TexturePath;

		EntityID GetEntityID() { return m_EntityID; }

		ModelComp() = default;
		ModelComp(const ModelComp&) = default;
		//Sets up model based on providde file path
		//Keep in mind that you should apply transforms to 3D meshes before exporting, or else meshes might be skewed or rotated weirdly
		ModelComp(const std::string& modelPath, EntityID entityID) : ModelPath(modelPath), m_EntityID(entityID)
		{
			GalaxyDraw::IModel::Create(modelPath, entityID);
		}

		~ModelComp() = default;
		

		//TODO when we create a model, it should be using the path provided and setup all values in m_Model,
		// later in the scene when we are updating we should get all modelComps that are set to be visible
		// and draw them using a static function in Renderer DrawModel().
		// look at ModelOld and MeshOld for reference of how it used to be done.
	private:
		EntityID m_EntityID;
	};
}