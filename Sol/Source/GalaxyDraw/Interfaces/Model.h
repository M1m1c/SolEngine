#pragma once
#include <string>
#include <memory>
#include "Mesh.h"
namespace GalaxyDraw 
{
	class Model
	{
	public:
		static std::shared_ptr<Model> Create(const std::string& modelpath); //TODO add optional texture path

		virtual ~Model() = default;	
		virtual void SetData(const std::string& path) = 0;
		virtual bool operator==(const Model& other) const = 0;
		virtual std::vector<Mesh> GetMeshes() const = 0;
		//virtual std::vector<MeshTexture> GetLoadedMaterials() const = 0;
	};

}