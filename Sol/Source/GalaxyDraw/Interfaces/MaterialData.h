#pragma once


#include "MeshRenderData.h"
#include "Shader.h"

#include "Sol/SolDefines.h"
#include "Sol/Core/KeyedVector.h"

namespace GalaxyDraw
{
	struct MaterialData
	{
		std::string Name;
		KeyedVector<std::string, MeshRenderData> MeshDataCollections;
		std::vector<EntityID> EntitiesUsingMat;
		std::string BaseTexturePath;
		std::shared_ptr<Shader> Shader;
	};
}