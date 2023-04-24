#include "solpch.h"
#include "IModel.h"
#include "Renderer3D.h"
#include "ModelManager.h"
#include "GalaxyDraw/Model.h"

namespace GalaxyDraw
{
	std::shared_ptr<IModel> IModel::Create(const std::string& path, EntityID entityID, uint32_t matIndex)
	{
		std::shared_ptr<IModel> retVal;

		auto& modelManager = ModelManager::GetInstance();
		retVal = modelManager.ProcessModel(path);

		if (retVal)
		{
			Renderer3D::LoadModel(retVal, entityID, matIndex);
			return retVal;
		}

		SOL_CORE_ASSERT(false, "No Model Found!");
		return nullptr;
	}
}

