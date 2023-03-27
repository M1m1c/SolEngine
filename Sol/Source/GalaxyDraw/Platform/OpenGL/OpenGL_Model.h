#pragma once
#include "GalaxyDraw/Interfaces/IModel.h"

struct aiNode;
struct aiScene;
struct aiMesh;

namespace GalaxyDraw 
{
	class OpenGL_Model : public IModel
	{
	public:
		OpenGL_Model(const std::string& modelpath);
		~OpenGL_Model();
		// Inherited via Model3D

		virtual void SetData(const std::string& path) override;

		virtual bool operator==(const IModel& other) const override;

		virtual uint32_t GetRendererID() const override { return m_RendererID; };

		virtual std::vector<std::shared_ptr<Mesh>>& GetMeshes() override { return m_Meshes; };

		virtual const std::string& GetName() override { return m_Name; }

	private:
		void LoadModel(const std::string& modelpath);
		void ProcessNode(aiNode* node, const aiScene* scene);
		std::shared_ptr<Mesh> ProcessMesh(aiMesh* mesh, const aiScene* scene, aiNode* node);

		uint32_t m_RendererID;
		std::string m_ModelDirectory;
		std::string m_Name;

		std::vector<std::shared_ptr<Mesh>> m_Meshes;
	};

	
}