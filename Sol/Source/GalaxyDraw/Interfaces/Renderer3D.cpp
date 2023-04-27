#include "solpch.h"
#include "Renderer3D.h"

#include "Buffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "IModel.h"
#include "Texture.h"
#include "UniformBuffer.h"
#include "RenderCommand.h"

#include "Sol/Core/KeyedVector.h"

#include "ModelManager.h"
#include "TextureManager.h"

#include "InstanceData.h"
#include "TextureManager.h"
#include "MeshRenderData.h"
#include "MaterialData.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace GalaxyDraw
{
	struct Renderer3DData
	{
		static const uint32_t MaxMeshes = 2000;
		static const uint32_t MaxTextureSlots = 32; // TODO: RenderCaps

		std::vector<std::shared_ptr<MaterialData>> MaterialDataCollections;
		uint32_t DefaultMaterialIndex = 0;

		KeyedVector<EntityID, std::string> EntityToModelPath;
		std::map<std::string, std::vector<uint32_t>> TextureToMaterialsMap = { {"",{0}} };
		Renderer3D::Statistics Stats;

		struct CameraData
		{
			glm::mat4 ViewProjection;
		};
		CameraData CameraBuffer;
		std::shared_ptr<UniformBuffer> CameraUniformBuffer;
	};

	static Renderer3DData s_3DData;

	void Renderer3D::Init()
	{
		SOL_PROFILE_FUNCTION();

		s_3DData.CameraUniformBuffer = UniformBuffer::Create(sizeof(Renderer3DData::CameraData), 0);

		auto& texManager = TextureManager::GetInstance();
		texManager.Initialize();

		std::shared_ptr<MaterialData> defaultMat = CreateMaterialData("Default", { "cube.vert", "cube.frag" }, "Default", "");
		s_3DData.MaterialDataCollections.push_back(defaultMat);
	}

	void Renderer3D::Shutdown()
	{
		SOL_PROFILE_FUNCTION();

		for (auto& matData : s_3DData.MaterialDataCollections)
		{
			auto uniqueMeshCount = matData->MeshDataCollections.size();
			auto& meshDataCollections = matData->MeshDataCollections;

			for (size_t i = 0; i < uniqueMeshCount; i++)
			{
				delete[] meshDataCollections[i].VertexBufferBase;
				delete[] meshDataCollections[i].InstanceBufferBase;
			}
		}
	}

	void Renderer3D::BeginScene(const glm::mat4& projection, const glm::mat4& view)
	{
		SOL_PROFILE_FUNCTION();

		s_3DData.CameraBuffer.ViewProjection = projection * view;
		s_3DData.CameraUniformBuffer->SetData(&s_3DData.CameraBuffer, sizeof(Renderer3DData::CameraData));

		//TODO add a way of seing draw calls in imgui
		//SOL_CORE_TRACE("DrawCalls = {0}", s_3DData.Stats.DrawCalls);
		s_3DData.Stats.DrawCalls = 0;

		Submit();
	}

	void Renderer3D::BeginScene(const OrthoCamera& camera)
	{
		SOL_PROFILE_FUNCTION();

		s_3DData.CameraBuffer.ViewProjection = camera.GetViewProjectionMatrix();
		s_3DData.CameraUniformBuffer->SetData(&s_3DData.CameraBuffer, sizeof(Renderer3DData::CameraData));

		s_3DData.Stats.DrawCalls = 0;

		Submit();
	}

	void Renderer3D::EndScene()
	{
		SOL_PROFILE_FUNCTION();

		Flush();
	}

	//Resets the buffer pointers for each mesh data entry
	void Renderer3D::Submit()
	{
		for (auto& matData : s_3DData.MaterialDataCollections)
		{
			auto uniqueMeshCount = matData->MeshDataCollections.size();
			auto& meshDataCollections = matData->MeshDataCollections;

			for (size_t i = 0; i < uniqueMeshCount; i++)
			{
				auto& meshData = meshDataCollections[i];

				meshData.VertexBufferPtr = meshData.VertexBufferBase;
				meshData.InstanceBufferPtr = meshData.InstanceBufferBase;
				//s_3DData.TextureSlotIndex = 1;
			}
		}


	}

	//sets buffer data and calls draw instanced.
	//each unique mesh results in a new draw call.
	void Renderer3D::Flush()
	{

		for (auto& matData : s_3DData.MaterialDataCollections)
		{
			if (matData->EntitiesUsingMat.size() == 0) { continue; }
			auto uniqueMeshCount = matData->MeshDataCollections.size();

			auto diffuseTexture = TextureManager::GetTexture(matData->BaseTexturePath);
			diffuseTexture->Bind(0);

			matData->Shader->Bind();
			matData->Shader->SetInt("u_Texture", 0);

			for (size_t i = 0; i < uniqueMeshCount; i++)
			{


				auto& meshData = matData->MeshDataCollections[i];
				if (meshData.m_Instances.size() > 0)
				{
					uint32_t dataSize = (uint32_t)((uint8_t*)meshData.VertexBufferPtr - (uint8_t*)meshData.VertexBufferBase);
					meshData.m_VertexBuffer->SetData(meshData.VertexBufferBase, dataSize);

					uint32_t instanceDataSize = (uint32_t)((uint8_t*)meshData.InstanceBufferPtr - (uint8_t*)meshData.InstanceBufferBase);
					meshData.m_InstanceBuffer->SetData(meshData.InstanceBufferBase, instanceDataSize);

					RenderCommand::DrawInstanced(meshData.m_VertexArray, meshData.m_Instances.size());
					s_3DData.Stats.DrawCalls++;
				}
			}
		}
	}

	std::shared_ptr<MaterialData> Renderer3D::CreateMaterialData(std::string matName, std::pair<std::string, std::string> shaderFiles, std::string shaderName, std::string texturePath)
	{
		std::shared_ptr<MaterialData> material = std::make_shared<MaterialData>();
		material->Name = matName;
		material->Shader = Shader::Create(shaderFiles.first, shaderFiles.second, shaderName);
		material->BaseTexturePath = texturePath;
		return material;
	}

	void Renderer3D::DiscardMeshInstances(EntityID entityID, std::shared_ptr<MaterialData> matData)
	{

		if (s_3DData.EntityToModelPath.Exists(entityID))
		{
			s_3DData.EntityToModelPath.eraseWithKey(entityID);
		}

		auto& meshDataCollection = matData->MeshDataCollections;


		for (int i = meshDataCollection.size() - 1; i >= 0; i--)
		{
			auto& meshData = meshDataCollection[i];
			if (!meshDataCollection[i].m_Instances.Exists(entityID)) { continue; }

			meshData.m_Instances.eraseWithKey(entityID);

			if (meshData.m_Instances.size() == 0)
			{
				delete[] meshData.VertexBufferBase;
				delete[] meshData.InstanceBufferBase;
				meshDataCollection.eraseWithIndex(i);
			}

		}
	}

	void Renderer3D::ReloadModel(std::string& modelPath, const EntityID& entityID, const uint32_t& materialIndex)
	{
		if (modelPath != "")
		{
			std::shared_ptr<Model> model = nullptr;
			if (ModelManager::IsModelLoaded(modelPath))
			{
				model = ModelManager::GetModel(modelPath);
			}
			else
			{
				model = ModelManager::ProcessModel(modelPath);
			}
			LoadModel(model, entityID, materialIndex);
		}
	}

	void Renderer3D::LoadTextureForMaterial(const std::string& texturePath, const uint32_t& materialIndex)
	{
		bool isTextureLoaded = TextureManager::IsTextureLoaded(texturePath);

		TextureManager::LoadTexture(texturePath);

		if (!isTextureLoaded)
		{
			s_3DData.TextureToMaterialsMap.insert(std::make_pair(texturePath, std::vector<uint32_t>({ materialIndex })));
		}
		else
		{
			s_3DData.TextureToMaterialsMap[texturePath].push_back(materialIndex);
		}
	}

	//Loads all sub meshes of a model
	//When we create a model on a modelComp using Model::Create() this also gets called.
	void Renderer3D::LoadModel(std::shared_ptr<IModel> model, EntityID entityID, uint32_t materialIndex)
	{
		SOL_PROFILE_FUNCTION();

		if (!s_3DData.EntityToModelPath.Exists(entityID))
		{
			s_3DData.EntityToModelPath.push_back(entityID, model->GetDir());
		}
		else
		{
			auto& path = s_3DData.EntityToModelPath.Get(entityID);
			path = model->GetDir();
		}

		auto& matData = s_3DData.MaterialDataCollections[materialIndex];

		auto& entitesUsingMat = matData->EntitiesUsingMat;

		auto iterator = std::find(entitesUsingMat.begin(), entitesUsingMat.end(), entityID);
		if (iterator == entitesUsingMat.end())
		{
			entitesUsingMat.push_back(entityID);
		}

		auto& meshes = model->GetMeshes();

		for (size_t i = 0; i < meshes.size(); i++)
		{
			LoadMesh(meshes[i], model->GetName(), entityID, materialIndex);
		}
	}

	//Sets up the mesh data for the mesh, buffers and vertex array.
	//Duplicate meshes get added to already exisisting mesh data.
	void Renderer3D::LoadMesh(const std::shared_ptr<Mesh>& mesh, const std::string& modelName, EntityID entityID, uint32_t materialIndex)
	{
		SOL_PROFILE_FUNCTION();

		auto& matData = s_3DData.MaterialDataCollections[materialIndex];
		auto& meshDataCollections = matData->MeshDataCollections;



		auto name = mesh->Name + "_" + modelName;
		//TODO check with model manager if mesh exists
		if (meshDataCollections.Exists(name))
		{
			auto& meshRenderData = meshDataCollections.Get(name);
			meshRenderData.m_Instances.push_back(entityID, InstanceData());
			return;
		}


		MeshRenderData meshData;
		meshData.m_Mesh = mesh;

		meshData.m_VertexArray = VertexArray::Create();
		meshData.m_VertexBuffer = VertexBuffer::Create(mesh->Vertices.size() * sizeof(Vertex));

		meshData.m_VertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position"     },
			{ ShaderDataType::Float3, "a_Normal"     },
			{ ShaderDataType::Float2, "a_TexCoord"     },
			{ ShaderDataType::Float4, "a_Color"     }
			});

		meshData.m_VertexArray->AddVertexBuffer(meshData.m_VertexBuffer);

		uint32_t maxVerts = mesh->Vertices.size();
		meshData.VertexBufferBase = new Vertex[maxVerts];
		meshData.InstanceBufferBase = new InstanceData[s_3DData.MaxMeshes];

		meshData.m_IndexBuffer = IndexBuffer::Create(mesh->Indices.data(), mesh->Indices.size());
		meshData.m_VertexArray->SetIndexBuffer(meshData.m_IndexBuffer);

		meshData.m_Instances.push_back(entityID, InstanceData());
		meshData.m_InstanceBuffer = InstanceBuffer::Create(s_3DData.MaxMeshes * sizeof(InstanceData));

		meshData.m_InstanceBuffer->SetLayout({
		{ ShaderDataType::Int, "a_EntityID"     },
		{ ShaderDataType::Int, "a_TextureID"     },
		{ ShaderDataType::Float4, "a_MeshColor"},
		{ ShaderDataType::Mat4 , "a_EntityTransform"},
		{ ShaderDataType::Mat4 , "a_MeshTransform"}
			});

		meshData.m_VertexArray->SetInstanceBuffer(meshData.m_InstanceBuffer);

		meshDataCollections.push_back(name, meshData);
	}

	uint32_t Renderer3D::UpdateExistingMaterial(const std::string& texturePath, const uint32_t materialIndex, const EntityID entityID)
	{

		auto& s = s_3DData;

		auto material = s_3DData.MaterialDataCollections[materialIndex];
		auto currentTexture = material->BaseTexturePath;
		if (currentTexture == texturePath) { return materialIndex; }

		//Removes material from the map to the texure it is currently using
		auto textureIT = s_3DData.TextureToMaterialsMap.find(currentTexture);
		if (textureIT != s_3DData.TextureToMaterialsMap.end())
		{
			std::vector<uint32_t>& matIndices = textureIT->second;
			auto indexIT = std::find(matIndices.begin(), matIndices.end(), materialIndex);
			if (indexIT != matIndices.end())
			{
				matIndices.erase(indexIT);
			}
		}

		LoadTextureForMaterial(texturePath, materialIndex);

		material->BaseTexturePath = texturePath;

		return materialIndex;
	}

	uint32_t Renderer3D::CreateNewMaterial(const std::string& texturePath, const EntityID entityID)
	{
		auto& s = s_3DData;

		auto size = s_3DData.MaterialDataCollections.size();

		uint32_t materialIndex = size;

		LoadTextureForMaterial(texturePath, materialIndex);

		std::string modelPath = DiscardEntityRenderData(entityID, false, false);

		std::string matName = "newMaterial";
		matName += std::to_string(size);

		std::shared_ptr<MaterialData> newMat = CreateMaterialData(matName, { "cube.vert", "cube.frag" }, "Default", texturePath);
		newMat->EntitiesUsingMat.push_back(entityID);
		s_3DData.MaterialDataCollections.push_back(newMat);


		ReloadModel(modelPath, entityID, materialIndex);

		return materialIndex;
	}

	uint32_t Renderer3D::CreateNewMaterial(const std::string& texturePath, std::string matName)
	{
		auto& s = s_3DData;

		auto size = s_3DData.MaterialDataCollections.size();

		uint32_t materialIndex = size;

		LoadTextureForMaterial(texturePath, materialIndex);

		if (matName == "") 
		{
			matName = "newMaterial";
			matName += std::to_string(size);
		}

		std::shared_ptr<MaterialData> newMat = CreateMaterialData(matName, { "cube.vert", "cube.frag" }, "Default", texturePath);
		s_3DData.MaterialDataCollections.push_back(newMat);

		return materialIndex;
	}

	uint32_t Renderer3D::SwapMaterial(const uint32_t matIndex, const EntityID entityID)
	{
		auto& s = s_3DData;

		//TODO check if index exists
		auto& matDataCollections = s.MaterialDataCollections;
		if (matIndex >= matDataCollections.size())
		{
			LoadTextureForMaterial(matDataCollections[s.DefaultMaterialIndex]->BaseTexturePath, s.DefaultMaterialIndex);

			return s.DefaultMaterialIndex;
		}

		std::string modelPath = DiscardEntityRenderData(entityID, false, false);

		LoadTextureForMaterial(matDataCollections[matIndex]->BaseTexturePath, matIndex);

		ReloadModel(modelPath, entityID, matIndex);

		return matIndex;
	}

	//TODO make sue we unladthe texture if all materials that use it are gone
	//Right now I am not entierly sure how we remove materials when we open a new scene
	void Renderer3D::DeleteMaterial(uint32_t materialIndex, std::function<void(uint32_t, EntityID)> function)
	{
		if (materialIndex == 0 && function != nullptr) { return; }
		auto& matDataCollections = s_3DData.MaterialDataCollections;

		if (materialIndex < matDataCollections.size())
		{
			auto& material = matDataCollections[materialIndex];
			for (int i = material->EntitiesUsingMat.size() - 1; i >= 0; i--)
			{
				auto entityID = material->EntitiesUsingMat[i];
				function(0, entityID);
			}
			matDataCollections.erase(matDataCollections.begin() + materialIndex);
		}
	}

	std::vector<std::shared_ptr<MaterialData>>& Renderer3D::GetMaterials()
	{
		return s_3DData.MaterialDataCollections;
	}


	std::string Renderer3D::DiscardEntityRenderData(const EntityID& entityID, bool shouldDiscardMaterial, bool shouldDiscardModel)
	{
		auto& texManager = TextureManager::GetInstance();
		auto& modelManager = ModelManager::GetInstance();
		std::string modelPath = "";

		for (int i = s_3DData.MaterialDataCollections.size() - 1; i >= 0; i--)
		{
			auto& matData = s_3DData.MaterialDataCollections[i];
			auto& entitiesUsingMat = matData->EntitiesUsingMat;

			auto iterator = std::find(entitiesUsingMat.begin(), entitiesUsingMat.end(), entityID);
			if (iterator != entitiesUsingMat.end())
			{
				if (s_3DData.EntityToModelPath.Exists(entityID))
				{
					modelPath = s_3DData.EntityToModelPath.Get(entityID);
				}
				entitiesUsingMat.erase(iterator);
				DiscardMeshInstances(entityID, matData);

				if (shouldDiscardMaterial)
					texManager.DiscardTextureInstance(matData->BaseTexturePath);

				if (shouldDiscardModel)
					modelManager.DiscardModelInstance(modelPath);
			}

			bool isNotDefaultMat = i != 0;
			if (isNotDefaultMat && shouldDiscardMaterial && entitiesUsingMat.empty())
			{
				s_3DData.MaterialDataCollections.erase(s_3DData.MaterialDataCollections.begin() + i);
			}
		}
		return modelPath;
	}

	uint32_t Renderer3D::GetMaterialIndex(EntityID entityID)
	{
		uint32_t retval = 0;
		for (size_t i = 0; i < s_3DData.MaterialDataCollections.size(); i++)
		{
			auto& matData = s_3DData.MaterialDataCollections[i];
			auto& entitesUsingMat = matData->EntitiesUsingMat;

			auto iterator = std::find(entitesUsingMat.begin(), entitesUsingMat.end(), entityID);
			if (iterator != entitesUsingMat.end())
			{
				retval = i;
				break;
			}
		}
		return retval;
	}

	std::shared_ptr<MaterialData> Renderer3D::GetMaterial(uint32_t materialIndex)
	{
		auto& matDataCollections = s_3DData.MaterialDataCollections;

		if (materialIndex < matDataCollections.size())
		{
			return matDataCollections[materialIndex];
		}
		return nullptr;
	}

	std::vector<std::shared_ptr<MaterialData>>& Renderer3D::GetAllMaterials()
	{
		return s_3DData.MaterialDataCollections;
	}

	//Iterates over all mesh data and updates their attributes
	void Renderer3D::DrawInstances()
	{
		SOL_PROFILE_FUNCTION();
		for (auto& matData : s_3DData.MaterialDataCollections)
		{
			auto& meshDataCollections = matData->MeshDataCollections;

			for (auto& meshData : meshDataCollections)
			{
				SOL_PROFILE_FUNCTION();

				auto& mesh = meshData.m_Mesh;
				uint32_t vertexCount = mesh->Vertices.size();

				for (size_t i = 0; i < vertexCount; i++)
				{
					auto& vert = mesh->Vertices[i];
					meshData.VertexBufferPtr->Position = glm::vec4(vert.Position, 0.f);
					meshData.VertexBufferPtr->Normal = vert.Normal;
					meshData.VertexBufferPtr->TexCoords = vert.TexCoords;
					meshData.VertexBufferPtr->Color = glm::vec4(1.f, 1.f, 1.f, 1.f);
					meshData.VertexBufferPtr++;
				}

				for (auto& id : matData->EntitiesUsingMat)
				{
					if (!meshData.m_Instances.Exists(id)) { continue; }
					auto& instanceData = meshData.m_Instances.Get(id);

					meshData.InstanceBufferPtr->m_EntityID = (int)id;
					meshData.InstanceBufferPtr->m_MeshColor = instanceData.m_MeshColor;
					meshData.InstanceBufferPtr->m_EntityTransform = instanceData.m_EntityTransform;
					meshData.InstanceBufferPtr->m_MeshTransform = instanceData.m_MeshTransform;
					meshData.InstanceBufferPtr++;
				}
			}
		}


	}

	//Updates instance specific data, based on entity components in scene.
	void Renderer3D::UpdateInstanceData(EntityID entityID, const InstanceData& instanceData)
	{
		for (auto& matData : s_3DData.MaterialDataCollections)
		{

			if (matData->EntitiesUsingMat.size() == 0) { continue; }

			bool containsEntityId = false;
			for (auto id : matData->EntitiesUsingMat)
			{
				if (id == entityID)
				{
					containsEntityId = true;
					break;
				}
			}

			if (!containsEntityId) { continue; }

			for (auto& meshData : matData->MeshDataCollections)
			{
				if (meshData.m_Instances.size() == 0) { continue; }
				if (!meshData.m_Instances.Exists(entityID)) { continue; }

				auto& collectionInstanceData = meshData.m_Instances.Get(entityID);
				collectionInstanceData = instanceData;
				//TODO fix so we can adjust the meshtransform per instance
				collectionInstanceData.m_MeshTransform = meshData.m_Mesh->MeshTransform;
			}
		}


	}



	void Renderer3D::ResetStats()
	{
		memset(&s_3DData.Stats, 0, sizeof(Statistics));
	}

	Renderer3D::Statistics Renderer3D::GetStats()
	{
		return s_3DData.Stats;
	}

}
