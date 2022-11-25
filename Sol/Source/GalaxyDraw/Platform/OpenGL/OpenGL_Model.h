#pragma once
#include "GalaxyDraw/Interfaces/Model.h"

struct aiNode;
struct aiScene;
struct aiMesh;

namespace GalaxyDraw 
{
	class OpenGL_Model : public Model
	{
	public:
		OpenGL_Model(const std::string& modelpath);

		// Inherited via Model3D

		virtual void SetData(const std::string& path) override;

		virtual bool operator==(const Model& other) const override;

		virtual uint32_t GetRendererID() const override { return m_RendererID; };

		virtual std::vector<Mesh>& GetMeshes() override { return m_Meshes; };

	private:
		void LoadModel(const std::string& modelpath);
		void ProcessNode(aiNode* node, const aiScene* scene);
		Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);

		uint32_t m_RendererID;
		std::string m_ModelDirectory;

		std::vector<Mesh> m_Meshes;
	};

	
}