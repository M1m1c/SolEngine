#include "solpch.h"
#include "IModel.h"
#include "Renderer.h"
#include "Renderer3D.h"
#include "GalaxyDraw/Platform/OpenGL/OpenGL_Model.h"

namespace GalaxyDraw
{
	std::shared_ptr<IModel> IModel::Create(const std::string& path, EntityID entityID)
	{
		std::shared_ptr<IModel> retVal;

		//TODO Actually we should check if this model is already loaded in another component when we load the file,
		// if it ise we should simply return a pointer to that model so we don't have to process the same model again

		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			SOL_CORE_ASSERT(false, "RendererAPI::None is not supported!");
			return nullptr;

		case RendererAPI::API::OpenGL:
			retVal = std::make_shared<OpenGL_Model>(path);
			break;

		}

		if (retVal) 
		{ 
			Renderer3D::LoadModel(retVal,entityID);
			return retVal; 
		}
		SOL_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}

