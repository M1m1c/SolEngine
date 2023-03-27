#pragma once
#include <string>
#include <memory>
#include "Mesh.h"
#include "Sol/SolDefines.h"

namespace GalaxyDraw 
{
	class IModel
	{
	public:
		static std::shared_ptr<IModel> Create(const std::string& modelpath, EntityID entityID); //TODO add optional texture path

		virtual ~IModel() = default;	
		virtual void SetData(const std::string& path) = 0;
		virtual bool operator==(const IModel& other) const = 0;
		virtual uint32_t GetRendererID() const = 0;
		virtual std::vector<std::shared_ptr<Mesh>>& GetMeshes() = 0;
		virtual const std::string& GetName() = 0;
		//virtual std::vector<MeshTexture> GetLoadedMaterials() const = 0;
	};

}