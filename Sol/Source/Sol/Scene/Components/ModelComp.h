#pragma once

#include <GalaxyDraw/Interfaces/Model.h>
#include <GalaxyDraw/Interfaces/Texture.h>
#include "Sol/SolDefines.h"

namespace Sol {
	struct ModelComp
	{
		s_ptr<GalaxyDraw::Model> Model;
		std::string ModelPath;
		std::string TexturePath;

		ModelComp() = default;
		ModelComp(const ModelComp&) = default;
		//Sets up model based on providde file path
		ModelComp(const std::string& modelPath, EntityID entityID) : ModelPath(modelPath)
		{
			Model = GalaxyDraw::Model::Create(modelPath, entityID);
		}

		//TODO when we create a model, it should be using the path provided and setup all values in m_Model,
		// later in the scene when we are updating we should get all modelComps that are set to be visible
		// and draw them using a static function in Renderer DrawModel().
		// look at ModelOld and MeshOld for reference of how it used to be done.
	};
}