#pragma once
#include "GalaxyDraw/Interfaces/Model.h"
namespace GalaxyDraw 
{
	class OpenGL_Model : public Model
	{
	public:
		OpenGL_Model(const std::string& modelpath);

		// Inherited via Model3D
		virtual void SetData(const std::string& path) override;

		virtual bool operator==(const Model& other) const override;

		virtual std::vector<Mesh> GetMeshes() const override;

	};

	
}