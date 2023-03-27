#include "solpch.h"
#include "OpenGL_Model.h"

#include "GalaxyDraw/Platform/OpenGL/GLMacros.h"
#include <glad/glad.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace GalaxyDraw
{
	OpenGL_Model::OpenGL_Model(const std::string& modelpath)
	{
		LoadModel(modelpath);
	}

	OpenGL_Model::~OpenGL_Model()
	{
		m_Meshes.clear();
	}

	void OpenGL_Model::SetData(const std::string& path)
	{
		LoadModel(path);
	}

	bool OpenGL_Model::operator==(const IModel& other) const
	{
		return m_RendererID == ((OpenGL_Model&)other).m_RendererID;
	}

	void OpenGL_Model::LoadModel(const std::string& modelpath)
	{
		Assimp::Importer import;
		const aiScene* scene = import.ReadFile(modelpath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals | aiProcess_JoinIdenticalVertices);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
			return;
		}
		m_ModelDirectory = modelpath.substr(0, modelpath.find_last_of('/'));

		m_Name = modelpath.substr(modelpath.find_last_of('/') + 1);

		ProcessNode(scene->mRootNode, scene);
	}

	void OpenGL_Model::ProcessNode(aiNode* node, const aiScene* scene)
	{
		//node->mt
		// process all the node's meshes (if any)
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			m_Meshes.push_back(ProcessMesh(mesh, scene, node));
		}
		// then do the same for each of its children
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			ProcessNode(node->mChildren[i], scene);
		}
	}

	std::shared_ptr<Mesh> OpenGL_Model::ProcessMesh(aiMesh* mesh, const aiScene* scene, aiNode* node)
	{
		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;
		std::vector<std::shared_ptr<Texture>> textures;


		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex vertex;

			glm::vec3 vector;
			vector.x = mesh->mVertices[i].x;
			vector.y = mesh->mVertices[i].y;
			vector.z = mesh->mVertices[i].z;
			vertex.Position = vector;

			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.Normal = vector;

			// does the mesh contain texture coordinates?
			if (mesh->mTextureCoords[0])
			{
				glm::vec2 vec;
				vec.x = mesh->mTextureCoords[0][i].x;
				vec.y = mesh->mTextureCoords[0][i].y;
				vertex.TexCoords = vec;
			}
			else
			{
				vertex.TexCoords = glm::vec2(0.0f, 0.0f);
			}

			vertices.push_back(vertex);
		}

		// LMAO, I wish C# procedural shit went like this
		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			for (unsigned int j = 0; j < face.mNumIndices; j++)
			{
				indices.push_back(face.mIndices[j]);
			}
		}

		// Checks if any material exists at all
		//if (mesh->mMaterialIndex >= 0)
		//{
		//	// gets the material cached within that specific index
		//	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		//	std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		//	textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		//	std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		//	textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		//}

		aiVector3D scale;
		aiVector3D rotation;
		aiVector3D position;
		node->mTransformation.Decompose(scale, rotation, position);

		glm::vec3 meshScale = glm::vec3(scale.x, scale.y, scale.z) * 0.01f;
		glm::vec3 meshRotation = glm::vec3(rotation.x , rotation.y, rotation.z);
		glm::vec3 meshPosition = glm::vec3(position.x, position.y, position.z) * 0.01f;

		auto transform = Sol::TransformComp(meshPosition, meshRotation, meshScale);

		return  std::make_shared<Mesh>(mesh->mName.C_Str(), vertices, indices, textures, transform);
	}

}

