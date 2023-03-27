#pragma once
#include "GalaxyDraw/Interfaces/IModel.h"

namespace GalaxyDraw 
{
	class Model : public IModel
	{
	public:
		Model(const std::string& modelDir, const std::string& name, std::vector<std::shared_ptr<Mesh>>& subMeshes) :
			m_ModelDirectory(modelDir), m_Name(name), m_Meshes(subMeshes)
		{

		}
		~Model(){ m_Meshes.clear(); }
		
		virtual std::vector<std::shared_ptr<Mesh>>& GetMeshes() override { return m_Meshes; };
		virtual const std::string& GetName() override { return m_Name; }
		virtual const std::string& GetDir() override { return m_ModelDirectory; }

	private:
		std::string m_ModelDirectory;
		std::string m_Name;

		std::vector<std::shared_ptr<Mesh>> m_Meshes;
	};

	
}