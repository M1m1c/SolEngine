#include "solpch.h"
#include "Renderer3D.h"

#include "Buffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "UniformBuffer.h"
#include "RenderCommand.h"
#include "MaterialManager.h"
#include "Sol/Core/KeyedVector.h"

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
		std::shared_ptr<Shader> Shader;

		KeyedVector<EntityID, InstanceData> m_Instances;
		std::vector<EntityID> m_ContainedEntityIds;

		Vertex* VertexBufferBase = nullptr;
		Vertex* VertexBufferPtr = nullptr;
		InstanceData* InstanceBufferBase = nullptr;
		InstanceData* InstanceBufferPtr = nullptr;
	};

	struct Renderer3DData
	{
		static const uint32_t MaxMeshes = 2000;
		static const uint32_t MaxTextureSlots = 32; // TODO: RenderCaps

		KeyedVector<std::string, MeshRenderData> MeshDataCollections;
		//std::shared_ptr<Texture2D> MissingTexture;

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

		auto& matManager = MaterialManager::GetInstance();
		matManager.Initialize(1,1,100, 0);
	}

	void Renderer3D::Shutdown()
	{
		SOL_PROFILE_FUNCTION();
		for (size_t i = 0; i < s_3DData.MeshDataCollections.size(); i++)
		{
			delete[] s_3DData.MeshDataCollections[i].VertexBufferBase;
			delete[] s_3DData.MeshDataCollections[i].InstanceBufferBase;
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
		for (size_t i = 0; i < s_3DData.MeshDataCollections.size(); i++)
		{
			auto& meshData = s_3DData.MeshDataCollections[i];

			meshData.VertexBufferPtr = meshData.VertexBufferBase;
			meshData.InstanceBufferPtr = meshData.InstanceBufferBase;
			//s_3DData.TextureSlotIndex = 1;
		}
	}

	//sets buffer data and calls draw instanced.
	//each unique mesh results in a new draw call.
	void Renderer3D::Flush()
	{
		auto uniqueMeshCount = s_3DData.MeshDataCollections.size();
		auto& matManager = MaterialManager::GetInstance();
		/*if (uniqueMeshCount > 0)
		{
			matManager.BindTextureArray();
		}*/

		for (size_t i = 0; i < uniqueMeshCount; i++)
		{


			auto& meshData = s_3DData.MeshDataCollections[i];
			if (meshData.m_Instances.size() > 0)
			{
				uint32_t dataSize = (uint32_t)((uint8_t*)meshData.VertexBufferPtr - (uint8_t*)meshData.VertexBufferBase);
				meshData.m_VertexBuffer->SetData(meshData.VertexBufferBase, dataSize);

				uint32_t instanceDataSize = (uint32_t)((uint8_t*)meshData.InstanceBufferPtr - (uint8_t*)meshData.InstanceBufferBase);
				meshData.m_InstanceBuffer->SetData(meshData.InstanceBufferBase, instanceDataSize);

				meshData.Shader->Bind();
				meshData.Shader->SetInt("u_Textures", matManager.GetTextureUnit());

				RenderCommand::DrawInstanced(meshData.m_VertexArray, meshData.m_Instances.size());
				s_3DData.Stats.DrawCalls++;
			}
		}
	}

	//Loads all sub meshes of a model
	//When we create a model on a modelComp using Model::Create() this also gets called.
	void Renderer3D::LoadModel(std::shared_ptr<IModel> model, EntityID entityID)
	{
		SOL_PROFILE_FUNCTION();
		auto& meshes = model->GetMeshes();

		for (size_t i = 0; i < meshes.size(); i++)
		{
			LoadMesh(meshes[i], model->GetName(), entityID);
		}
	}

	//Sets up the mesh data for the mesh, buffers and vertex array.
	//Duplicate meshes get added to already exisisting mesh data.
	void Renderer3D::LoadMesh(const std::shared_ptr<Mesh>& mesh, const std::string& modelName, EntityID entityID)
	{
		SOL_PROFILE_FUNCTION();
		auto name = mesh->Name + "_" + modelName;

		if (s_3DData.MeshDataCollections.Exists(name))
		{
			auto& meshRenderData = s_3DData.MeshDataCollections.Get(name);
			meshRenderData.m_Instances.push_back(entityID, InstanceData());
			meshRenderData.m_ContainedEntityIds.push_back(entityID);
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
		meshData.Shader = Shader::Create("cube.vert", "cube.frag", "Default");//TODO replace this with something we set in the material

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
		meshData.m_ContainedEntityIds.push_back(entityID);

		s_3DData.MeshDataCollections.push_back(name, meshData);
	}

	//Iterates over all mesh data and updates their attributes
	void Renderer3D::DrawInstances()
	{
		SOL_PROFILE_FUNCTION();
		for (auto& renderData : s_3DData.MeshDataCollections)
		{
			SOL_PROFILE_FUNCTION();

			auto& mesh = renderData.m_Mesh;
			uint32_t vertexCount = mesh->Vertices.size();

			for (size_t i = 0; i < vertexCount; i++)
			{
				auto& vert = mesh->Vertices[i];
				renderData.VertexBufferPtr->Position = glm::vec4(vert.Position, 0.f);
				renderData.VertexBufferPtr->Normal = vert.Normal;
				renderData.VertexBufferPtr->TexCoords = vert.TexCoords;
				renderData.VertexBufferPtr->Color = glm::vec4(1.f, 1.f, 1.f, 1.f);
				renderData.VertexBufferPtr++;
			}

			for (auto& id : renderData.m_ContainedEntityIds)
			{
				auto& instanceData = renderData.m_Instances.Get(id);

				renderData.InstanceBufferPtr->m_EntityID = (int)id;
				renderData.InstanceBufferPtr->m_MeshColor = instanceData.m_MeshColor;
				renderData.InstanceBufferPtr->m_EntityTransform = instanceData.m_EntityTransform;
				renderData.InstanceBufferPtr->m_MeshTransform = instanceData.m_MeshTransform;
				renderData.InstanceBufferPtr++;
			}

			//Mesh count needs to be reset
			//s_3DData.Stats.MeshCount++;
		}

	}

	//Updates instance specific data, based on entity components in scene.
	void Renderer3D::UpdateInstanceData(EntityID entityID, const InstanceData& instanceData)
	{
		for (auto& collection : s_3DData.MeshDataCollections)
		{
			if (collection.m_Instances.size() == 0) { continue; }

			bool containsEntityId = false;
			for (auto id : collection.m_ContainedEntityIds)
			{
				if (id == entityID)
				{
					containsEntityId = true;
					break;
				}
			}

			if (!containsEntityId) { continue; }
			auto& collectionData = collection.m_Instances.Get(entityID);
			collectionData = instanceData;
			//TODO fix so we can adjust the meshtransform per instance
			collectionData.m_MeshTransform = collection.m_Mesh->MeshTransform;
		}
	}

	//Removes all the model's mesh instances tied to the entityID from the MeshDataCollections
	void Renderer3D::DiscardMeshInstances(EntityID entityID, std::shared_ptr<IModel> model)
	{
		auto modelName = model->GetName();

		auto& meshes = model->GetMeshes();

		for (size_t i = 0; i < meshes.size(); i++)
		{
			auto name = meshes[i]->Name + "_" + modelName;



			if (s_3DData.MeshDataCollections.Exists(name))
			{
				auto& meshRenderData = s_3DData.MeshDataCollections.Get(name);
				meshRenderData.m_Instances.eraseWithKey(entityID);

				auto iterator = std::find(meshRenderData.m_ContainedEntityIds.begin(), meshRenderData.m_ContainedEntityIds.end(), entityID);
				if (iterator != meshRenderData.m_ContainedEntityIds.end())
				{
					meshRenderData.m_ContainedEntityIds.erase(iterator);
				}

				//TODO if there are no more instances in the mesh render data then unload the mesh and delete mesh render data
				if (meshRenderData.m_Instances.size() == 0)
				{
					//TODO figure out how to dealocate the loaded mesh

					delete[] meshRenderData.VertexBufferBase;
					delete[] meshRenderData.InstanceBufferBase;
					s_3DData.MeshDataCollections.eraseWithKey(name);
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
