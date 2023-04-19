#include "solpch.h"
#include "Renderer3D.h"

#include "Buffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"
#include "UniformBuffer.h"
#include "RenderCommand.h"
//#include "MaterialManager.h"
//#include "Sol/Core/KeyedVector.h"
#include "TextureManager.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace GalaxyDraw
{


	//for each unique mesh one of these structs are created,
	//it contains the relevant buffers and data for rendering instances of said mesh.
	struct MeshRenderData
	{
		std::string Name;
		std::shared_ptr<Mesh> m_Mesh;
		std::shared_ptr<VertexArray> m_VertexArray;
		std::shared_ptr<VertexBuffer> m_VertexBuffer;
		std::shared_ptr<IndexBuffer> m_IndexBuffer;
		std::shared_ptr<InstanceBuffer> m_InstanceBuffer;
		//std::shared_ptr<Shader> Shader;//TODO remove

		KeyedVector<EntityID, InstanceData> m_Instances;
		//std::vector<EntityID> m_ContainedEntityIds;

		Vertex* VertexBufferBase = nullptr;
		Vertex* VertexBufferPtr = nullptr;
		InstanceData* InstanceBufferBase = nullptr;
		InstanceData* InstanceBufferPtr = nullptr;
	};


	struct MaterialData
	{
		std::string Name;
		KeyedVector<std::string, MeshRenderData> MeshDataCollections;
		std::vector<EntityID> EntitiesUsingMat;
		std::string DiffuseTexturePath;
		std::shared_ptr<Shader> Shader;
	};

	//TODO we should restructure how we do our instanced rendering.
	// Currently each unique mesh determines teh amount od draw calls.
	// each unique mesh also holds onto an instance of the shader.
	// What we instead would want to do is have Materials hold onto shaders and a collection of MeshRenderData.
	// this way we would have an easier time of rebinding textures before the drawcall is issued,
	// additionally drawcalls would be dependent on unique materials and then unique meshes.
	// another cool thing we could do is have a collection of active materials and one of inactive,
	// when we remove instances we check if it is the last instance that uses that material and then we free it,
	// then if there are no more meshes of any kind we remove it form active, then after a set number of time we discard/unload all inactive materials,
	// freeing up the memory on the gpu, alternativley we frre them up imedietly when the last mesh using the material has been deleted.
	// btw we might be able to use the regular texture pipleline instead of using texture arrays,
	// since each material will bind those texutres to its shader.
	struct Renderer3DData
	{
		static const uint32_t MaxMeshes = 2000;
		static const uint32_t MaxTextureSlots = 32; // TODO: RenderCaps

		//KeyedVector<std::string, MeshRenderData> MeshDataCollections;
		//std::shared_ptr<Texture2D> MissingTexture;
		std::vector<MaterialData> MaterialDataCollections;

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

		/*	s_3DData.MissingTexture = Texture2D::Create(1, 1);
			uint32_t missingTextureData = 0xff00ff;
			s_3DData.MissingTexture->SetData(&missingTextureData, sizeof(uint32_t));*/

		s_3DData.CameraUniformBuffer = UniformBuffer::Create(sizeof(Renderer3DData::CameraData), 0);

	/*	auto& texManager = TextureManager::GetInstance();
		texManager.Initialize();*/

		MaterialData defaultMat;
		defaultMat.Name = "Default";
		defaultMat.Shader = Shader::Create("cube.vert", "cube.frag", "Default");
		defaultMat.DiffuseTexturePath = "";
		s_3DData.MaterialDataCollections.push_back(defaultMat);

		//TODO remove
	/*	auto& matManager = MaterialManager::GetInstance();
		matManager.Initialize(1, 0);*/
	}

	void Renderer3D::Shutdown()
	{
		SOL_PROFILE_FUNCTION();

		for (auto& matData : s_3DData.MaterialDataCollections)
		{
			auto uniqueMeshCount = matData.MeshDataCollections.size();
			auto& meshDataCollections = matData.MeshDataCollections;

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

		Submit();
	}

	void Renderer3D::BeginScene(const OrthoCamera& camera)
	{
		SOL_PROFILE_FUNCTION();

		s_3DData.CameraBuffer.ViewProjection = camera.GetViewProjectionMatrix();
		s_3DData.CameraUniformBuffer->SetData(&s_3DData.CameraBuffer, sizeof(Renderer3DData::CameraData));

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
			auto uniqueMeshCount = matData.MeshDataCollections.size();
			auto& meshDataCollections = matData.MeshDataCollections;

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
			auto uniqueMeshCount = matData.MeshDataCollections.size();
			//auto& matManager = MaterialManager::GetInstance();
			/*if (uniqueMeshCount > 0)
			{
				matManager.BindTextureArray();
			}*/


			//TODO ok so here is where we need to iterate over materials instead, and then over unique meshes that use thos materials
			for (size_t i = 0; i < uniqueMeshCount; i++)
			{


				auto& meshData = matData.MeshDataCollections[i];
				if (meshData.m_Instances.size() > 0)
				{
					uint32_t dataSize = (uint32_t)((uint8_t*)meshData.VertexBufferPtr - (uint8_t*)meshData.VertexBufferBase);
					meshData.m_VertexBuffer->SetData(meshData.VertexBufferBase, dataSize);

					uint32_t instanceDataSize = (uint32_t)((uint8_t*)meshData.InstanceBufferPtr - (uint8_t*)meshData.InstanceBufferBase);
					meshData.m_InstanceBuffer->SetData(meshData.InstanceBufferBase, instanceDataSize);

					matData.Shader->Bind();
					//TODO set textures here
					//matData.Shader->SetInt("u_Textures", matManager.GetTextureUnit());

					RenderCommand::DrawInstanced(meshData.m_VertexArray, meshData.m_Instances.size());
					s_3DData.Stats.DrawCalls++;
				}
			}
		}
		
	}

	//Loads all sub meshes of a model
	//When we create a model on a modelComp using Model::Create() this also gets called.
	void Renderer3D::LoadModel(std::shared_ptr<IModel> model, EntityID entityID, uint32_t materialIndex)
	{
		SOL_PROFILE_FUNCTION();
		auto& meshes = model->GetMeshes();

		for (size_t i = 0; i < meshes.size(); i++)
		{
			LoadMesh(meshes[i], model->GetName(), entityID);

			auto& matData = s_3DData.MaterialDataCollections[materialIndex];
			//TODO don't forget to erase it when the entity no longer exists
			matData.EntitiesUsingMat.push_back(entityID);
		}
	}

	//Sets up the mesh data for the mesh, buffers and vertex array.
	//Duplicate meshes get added to already exisisting mesh data.
	void Renderer3D::LoadMesh(const std::shared_ptr<Mesh>& mesh, const std::string& modelName, EntityID entityID, uint32_t materialIndex)
	{
		SOL_PROFILE_FUNCTION();

		auto& matData = s_3DData.MaterialDataCollections[materialIndex];
		auto& meshDataCollections = matData.MeshDataCollections;

		

		auto name = mesh->Name + "_" + modelName;
		//TODO check with model manager if mesh exists
		if (meshDataCollections.Exists(name))
		{
			auto& meshRenderData = meshDataCollections.Get(name);
			meshRenderData.m_Instances.push_back(entityID, InstanceData());
			//meshRenderData.m_ContainedEntityIds.push_back(entityID);
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


		//TODO should use missing texture to color 3d mesh
		//meshData.Shader = Shader::Create("cube.vert", "cube.frag", "Default");//TODO replace this with something we set in the material

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
		//meshData.m_ContainedEntityIds.push_back(entityID);

		meshDataCollections.push_back(name, meshData);
	}

	//Iterates over all mesh data and updates their attributes
	void Renderer3D::DrawInstances()
	{
		SOL_PROFILE_FUNCTION();
		for (auto& matData : s_3DData.MaterialDataCollections)
		{
			auto& meshDataCollections = matData.MeshDataCollections;

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

				for (auto& id : matData.EntitiesUsingMat)
				{
					auto& instanceData = meshData.m_Instances.Get(id);

					meshData.InstanceBufferPtr->m_EntityID = (int)id;
					meshData.InstanceBufferPtr->m_MeshColor = instanceData.m_MeshColor;
					meshData.InstanceBufferPtr->m_EntityTransform = instanceData.m_EntityTransform;
					meshData.InstanceBufferPtr->m_MeshTransform = instanceData.m_MeshTransform;
					meshData.InstanceBufferPtr++;
				}

				//Mesh count needs to be reset
				//s_3DData.Stats.MeshCount++;
			}
		}


	}

	//Updates instance specific data, based on entity components in scene.
	void Renderer3D::UpdateInstanceData(EntityID entityID, const InstanceData& instanceData)
	{
		for (auto& matData : s_3DData.MaterialDataCollections)
		{

			if (matData.EntitiesUsingMat.size() == 0) { continue; }

			bool containsEntityId = false;
			for (auto id : matData.EntitiesUsingMat)
			{
				if (id == entityID)
				{
					containsEntityId = true;
					break;
				}
			}

			if (!containsEntityId) { continue; }

			for (auto& meshData : matData.MeshDataCollections)
			{
				if (meshData.m_Instances.size() == 0) { continue; }

				/*bool containsEntityId = false;
				for (auto id : collection.m_ContainedEntityIds)
				{
					if (id == entityID)
					{
						containsEntityId = true;
						break;
					}
				}

				if (!containsEntityId) { continue; }*/
				auto& collectionInstanceData = meshData.m_Instances.Get(entityID);
				collectionInstanceData = instanceData;
				//TODO fix so we can adjust the meshtransform per instance
				collectionInstanceData.m_MeshTransform = meshData.m_Mesh->MeshTransform;
			}
		}

		
	}

	//Removes all the model's mesh instances tied to the entityID from the MeshDataCollections
	void Renderer3D::DiscardMeshInstances(EntityID entityID, std::shared_ptr<IModel> model)
	{
		auto modelName = model->GetName();

		auto& meshes = model->GetMeshes();

		for (auto& matData : s_3DData.MaterialDataCollections)
		{

			if (matData.EntitiesUsingMat.size() == 0) { continue; }

			auto& entitiesUsingMat = matData.EntitiesUsingMat;

			bool containsEntityId = false;
			for (auto id : entitiesUsingMat)
			{
				if (id == entityID)
				{
					containsEntityId = true;
					break;
				}
			}

			if (!containsEntityId) { continue; }

			auto iterator = std::find(entitiesUsingMat.begin(), entitiesUsingMat.end(), entityID);
			if (iterator != entitiesUsingMat.end())
			{
				entitiesUsingMat.erase(iterator);
			}

			auto& meshDataCollection = matData.MeshDataCollections;
			for (size_t i = 0; i < meshes.size(); i++)
			{
				auto name = meshes[i]->Name + "_" + modelName;

				if (meshDataCollection.Exists(name))
				{
					auto& meshRenderData = meshDataCollection.Get(name);
					meshRenderData.m_Instances.eraseWithKey(entityID);

					
					//TODO if there are no more instances in the mesh render data then unload the mesh and delete mesh render data
					if (meshRenderData.m_Instances.size() == 0)
					{
						//TODO figure out how to dealocate the loaded mesh

						delete[] meshRenderData.VertexBufferBase;
						delete[] meshRenderData.InstanceBufferBase;
						meshDataCollection.eraseWithKey(name);
					}
				}
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
