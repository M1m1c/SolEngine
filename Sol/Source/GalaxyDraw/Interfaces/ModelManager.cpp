#include "solpch.h"
#include "ModelManager.h"

#include <glad/glad.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace GalaxyDraw 
{
	void ModelManager::DiscardModel(const std::string& modelpath)
	{
		auto& s = ModelManager::GetInstance();
		if (s.m_LoadedModels.Exists(modelpath))
		{
			auto& modelInstance = s.m_LoadedModels.Get(modelpath);
			modelInstance.Count--;
			if (modelInstance.Count == 0) 
			{
				s.m_LoadedModels.eraseWithKey(modelpath);
			}
		}
	}

	std::shared_ptr<Model> ModelManager::ProcessModel(const std::string& modelpath)
	{
		auto& s = ModelManager::GetInstance();
		if (s.m_LoadedModels.Exists(modelpath)) 
		{
			auto& modelInstance = s.m_LoadedModels.Get(modelpath);
			modelInstance.Count++;
			return modelInstance.Model;
		}

		Assimp::Importer import;
		const aiScene* scene = import.ReadFile(modelpath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals | aiProcess_JoinIdenticalVertices);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			SOL_CORE_ASSERT(false, "ERROR::ASSIMP:: {0}", import.GetErrorString());
			return nullptr;
		}
		auto path = modelpath.substr(0, modelpath.find_last_of('/'));

		auto name = modelpath.substr(modelpath.find_last_of('/') + 1);

		std::vector<std::shared_ptr<Mesh>> meshes = {};
		ProcessNode(scene->mRootNode, scene,meshes);

		if (meshes.size() > 0) 
		{
			auto model = std::make_shared<Model>(path, name, meshes);
			s.m_LoadedModels.push_back(modelpath, ModelInstanceData(1,model));
			return model;
		}

		SOL_CORE_ASSERT(false, "No Meshes Found!");
		return nullptr;
	}

	std::shared_ptr<Model> ModelManager::GetModel(const std::string& path)
	{
		auto& s = ModelManager::GetInstance();
		if (s.m_LoadedModels.Exists(path))
		{
			return s.m_LoadedModels.Get(path).Model;
		}

		SOL_CORE_ASSERT(false, "Model not found!");
		return nullptr;
	}

	void ModelManager::ProcessNode(aiNode* node, const aiScene* scene, std::vector<std::shared_ptr<Mesh>>& meshes)
	{
		//node->mt
		// process all the node's meshes (if any)
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			meshes.push_back(ProcessMesh(mesh, scene, node));
		}
		// then do the same for each of its children
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			ProcessNode(node->mChildren[i], scene, meshes);
		}
	}

	std::shared_ptr<Mesh> ModelManager::ProcessMesh(aiMesh* mesh, const aiScene* scene, aiNode* node)
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
		glm::vec3 meshRotation = glm::vec3(rotation.x, rotation.y, rotation.z);
		glm::vec3 meshPosition = glm::vec3(position.x, position.y, position.z) * 0.01f;

		auto transform = Sol::TransformComp(meshPosition, meshRotation, meshScale);

		return  std::make_shared<Mesh>(mesh->mName.C_Str(), vertices, indices, textures, transform);
	}
}
