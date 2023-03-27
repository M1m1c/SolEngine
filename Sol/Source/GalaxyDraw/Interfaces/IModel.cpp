#include "solpch.h"
#include "IModel.h"
#include "Renderer3D.h"
#include "ModelManager.h"
#include "GalaxyDraw/Model.h"

namespace GalaxyDraw
{
	std::shared_ptr<IModel> IModel::Create(const std::string& path, EntityID entityID)
	{
		std::shared_ptr<IModel> retVal;

		//TODO Actually we should check if this model is already loaded in another component when we load the file,
		// if it ise we should simply return a pointer to that model so we don't have to process the same model again
		auto& modelManager = ModelManager::GetInstance();
		retVal = modelManager.ProcessModel(path);

		if (retVal) 
		{ 
			Renderer3D::LoadModel(retVal,entityID);
			return retVal; 
		}

		SOL_CORE_ASSERT(false, "No Model Found!");
		return nullptr;
	}
}

