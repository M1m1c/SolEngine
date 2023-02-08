#include "solpch.h"
#include "Renderer3D.h"

#include "Buffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "UniformBuffer.h"
#include "RenderCommand.h"
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
		std::shared_ptr<Texture2D> MissingTexture;

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

		s_3DData.MissingTexture = Texture2D::Create(1, 1);
		uint32_t missingTextureData = 0xff00ff;
		s_3DData.MissingTexture->SetData(&missingTextureData, sizeof(uint32_t));

		s_3DData.CameraUniformBuffer = UniformBuffer::Create(sizeof(Renderer3DData::CameraData), 0);
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

	void Renderer3D::BeginScene(const glm::mat4& projection, const glm::mat4& transform)
	{
		SOL_PROFILE_FUNCTION();

		s_3DData.CameraBuffer.ViewProjection = projection * glm::inverse(transform);
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
		for (size_t i = 0; i < s_3DData.MeshDataCollections.size(); i++)
		{
			auto& meshData = s_3DData.MeshDataCollections[i];
			if (meshData.m_Instances.size() > 0)
			{
				uint32_t dataSize = (uint32_t)((uint8_t*)meshData.VertexBufferPtr - (uint8_t*)meshData.VertexBufferBase);
				meshData.m_VertexBuffer->SetData(meshData.VertexBufferBase, dataSize);

				uint32_t instanceDataSize = (uint32_t)((uint8_t*)meshData.InstanceBufferPtr - (uint8_t*)meshData.InstanceBufferBase);
				meshData.m_InstanceBuffer->SetData(meshData.InstanceBufferBase, instanceDataSize);

				//// Bind textures
				//for (uint32_t i = 0; i < s_3DData.TextureSlotIndex; i++)
				//	s_3DData.TextureSlots[i]->Bind(i);

				meshData.Shader->Bind();
				RenderCommand::DrawInstanced(meshData.m_VertexArray,meshData.m_Instances.size());
				s_3DData.Stats.DrawCalls++;
			}
		}
	}

	//Loads all sub meshes of a model
	//When we create a model on a modelComp using Model::Create() this also gets called.
	void Renderer3D::LoadModel(std::shared_ptr<Model> model, EntityID entityID)
	{
		SOL_PROFILE_FUNCTION();
		auto& meshes = model->GetMeshes();

		for (size_t i = 0; i < meshes.size(); i++)
		{
			LoadMesh(meshes[i],model->GetName(),entityID);
		}
	}

	//Sets up the mesh data for the mesh, buffers and vertex array.
	//Duplicate meshes get added to already exisisting mesh data.
	void Renderer3D::LoadMesh(const std::shared_ptr<Mesh>& mesh,const std::string& modelName, EntityID entityID)
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

		uint32_t maxVerts = s_3DData.MaxMeshes * mesh->Vertices.size();
		uint32_t maxIndices = s_3DData.MaxMeshes * mesh->Indices.size();

		MeshRenderData meshData;
		meshData.m_Mesh = mesh;

		meshData.m_VertexArray = VertexArray::Create();
		meshData.m_VertexBuffer= VertexBuffer::Create(mesh->Vertices.size() * sizeof(Vertex));

		meshData.m_VertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position"     },
			{ ShaderDataType::Float3, "a_Normal"     },
			{ ShaderDataType::Float2, "a_TexCoord"     },
			{ ShaderDataType::Float4, "a_Color"     },
			{ ShaderDataType::Int, "a_EntityID"     }
			});

		meshData.m_VertexArray->AddVertexBuffer(meshData.m_VertexBuffer);

		meshData.VertexBufferBase = new Vertex[maxVerts];
		meshData.InstanceBufferBase = new InstanceData[s_3DData.MaxMeshes];

		meshData.m_IndexBuffer = IndexBuffer::Create(mesh->Indices.data(), mesh->Indices.size());
		meshData.m_VertexArray->SetIndexBuffer(meshData.m_IndexBuffer);


		//TODO should use missing texture to color 3d mesh
		meshData.Shader = Shader::Create("cube.vert", "cube.frag", "Default");//TODO replace this with something we set in the material

		meshData.m_Instances.push_back(entityID, InstanceData());
		meshData.m_InstanceBuffer = InstanceBuffer::Create(s_3DData.MaxMeshes * sizeof(InstanceData));

		meshData.m_InstanceBuffer->SetLayout({
		{ ShaderDataType::Float3, "A_MeshPosition"     }
			});

		meshData.m_VertexArray->SetInstanceBuffer(meshData.m_InstanceBuffer);
		meshData.m_ContainedEntityIds.push_back(entityID);

		s_3DData.MeshDataCollections.push_back(name, meshData);
	}

	//Iterates over all mesh data and updates their attributes
	void Renderer3D::DrawInstances()
	{
		SOL_PROFILE_FUNCTION();
		for (auto& renderData :s_3DData.MeshDataCollections)
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
				renderData.VertexBufferPtr->Color = glm::vec4(1.f, 0.f, 0.f, 1.f);
				renderData.VertexBufferPtr->EntityID = 0;
				renderData.VertexBufferPtr++;
			}
			
			for (auto& instanceData : renderData.m_Instances)
			{
				renderData.InstanceBufferPtr->MeshPosition = instanceData.MeshPosition;
				renderData.InstanceBufferPtr++;
			}

			//Mesh count needs to be reset
			//s_3DData.Stats.MeshCount++;
		}
	
	}

	void Renderer3D::UpdateInstanceData(EntityID entityID, const InstanceData& instanceData)
	{
		for (auto& collection : s_3DData.MeshDataCollections)
		{
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
			collection.m_Instances.Get(entityID) = instanceData;
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
