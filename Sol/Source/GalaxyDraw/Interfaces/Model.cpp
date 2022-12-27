#include "solpch.h"
#include "Model.h"
#include "Renderer.h"
#include "Renderer3D.h"
#include "GalaxyDraw/Platform/OpenGL/OpenGL_Model.h"

namespace GalaxyDraw
{
	std::shared_ptr<Model> Model::Create(const std::string& path)
	{
		std::shared_ptr<Model> retVal;

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
			Renderer3D::LoadModel(retVal);
			return retVal; 
		}
		SOL_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}

