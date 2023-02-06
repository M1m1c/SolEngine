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
	

	//TODO make sure that only one of these exist per unique mesh, if more meshes of the same are loaded add their data to the buffers
	// might want to store a map of string to id, pass in the name of the mesh file and get the mes render data name.
	struct MeshRenderData 
	{
		std::string Name;
		std::shared_ptr<Mesh> m_Mesh;
		std::shared_ptr<VertexArray> m_VertexArray;
		std::shared_ptr<VertexBuffer> m_VertexBuffer;
		std::shared_ptr<IndexBuffer> m_IndexBuffer;
		std::shared_ptr<InstanceBuffer> m_InstanceBuffer;
		std::shared_ptr<Shader> Shader;

		//TODO maybe eash instance needs its own vertex array where we bind the transform values
		// ´Take a look at following links
		//https://learnopengl.com/Advanced-OpenGL/Instancing
		//https://learnopengl.com/code_viewer_gh.php?code=src/4.advanced_opengl/10.3.asteroids_instanced/asteroids_instanced.cpp
		KeyedVector<uint32_t, InstanceData> m_Instances;
		//std::vector<InstanceData> Instances;

		std::vector<uint32_t> m_ContainedEntityIds;

		uint32_t IndexCount = 0;
		uint32_t IndexOffset = 0;
		uint32_t MaxIndicies = 0;
		uint32_t MaxVerts = 0;
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

	void GalaxyDraw::Renderer3D::Init()
	{
		SOL_PROFILE_FUNCTION();

		s_3DData.MissingTexture = Texture2D::Create(1, 1);
		uint32_t missingTextureData = 0xff00ff;
		s_3DData.MissingTexture->SetData(&missingTextureData, sizeof(uint32_t));

		s_3DData.CameraUniformBuffer = UniformBuffer::Create(sizeof(Renderer3DData::CameraData), 0);
	}

	void GalaxyDraw::Renderer3D::Shutdown()
	{
		SOL_PROFILE_FUNCTION();
		for (size_t i = 0; i < s_3DData.MeshDataCollections.size(); i++)
		{
			delete[] s_3DData.MeshDataCollections[i].VertexBufferBase;
			delete[] s_3DData.MeshDataCollections[i].InstanceBufferBase;
		}
		
	}

	void GalaxyDraw::Renderer3D::BeginScene(const glm::mat4& projection, const glm::mat4& transform)
	{
		SOL_PROFILE_FUNCTION();

		s_3DData.CameraBuffer.ViewProjection = projection * glm::inverse(transform);
		s_3DData.CameraUniformBuffer->SetData(&s_3DData.CameraBuffer, sizeof(Renderer3DData::CameraData));

		StartBatch();
	}

	void GalaxyDraw::Renderer3D::BeginScene(const OrthoCamera& camera)
	{
		SOL_PROFILE_FUNCTION();

		s_3DData.CameraBuffer.ViewProjection = camera.GetViewProjectionMatrix();
		s_3DData.CameraUniformBuffer->SetData(&s_3DData.CameraBuffer, sizeof(Renderer3DData::CameraData));

		StartBatch();
	}

	void GalaxyDraw::Renderer3D::EndScene()
	{
		SOL_PROFILE_FUNCTION();

		Flush();
	}

	//TODO we have a problem here with the mesh data colleciton, since it needs to be able to be iterated over as well
	//we should probaably create a new container type where we can access via keys and iterate over it using indices
	void Renderer3D::StartBatch()
	{
		for (size_t i = 0; i < s_3DData.MeshDataCollections.size(); i++)
		{
			auto& meshData = s_3DData.MeshDataCollections[i];

			meshData.IndexCount = 0;
			meshData.VertexBufferPtr = meshData.VertexBufferBase;
			meshData.InstanceBufferPtr = meshData.InstanceBufferBase;
			//s_3DData.TextureSlotIndex = 1;
		}
	}

	void GalaxyDraw::Renderer3D::Flush()
	{
		for (size_t i = 0; i < s_3DData.MeshDataCollections.size(); i++)
		{
			auto& meshData = s_3DData.MeshDataCollections[i];
			if (meshData.IndexCount)
			{
				uint32_t dataSize = (uint32_t)((uint8_t*)meshData.VertexBufferPtr - (uint8_t*)meshData.VertexBufferBase);
				meshData.m_VertexBuffer->SetData(meshData.VertexBufferBase, dataSize);

				uint32_t instanceDataSize = (uint32_t)((uint8_t*)meshData.InstanceBufferPtr - (uint8_t*)meshData.InstanceBufferBase);
				meshData.m_InstanceBuffer->SetData(meshData.InstanceBufferBase, instanceDataSize);

				//// Bind textures
				//for (uint32_t i = 0; i < s_3DData.TextureSlotIndex; i++)
				//	s_3DData.TextureSlots[i]->Bind(i);

				meshData.Shader->Bind();
				RenderCommand::DrawInstanced(meshData.m_VertexArray);
				//RenderCommand::DrawIndexed(meshData.m_VertexArray, meshData.IndexCount);
				s_3DData.Stats.DrawCalls++;
			}
		}
	}

	void Renderer3D::NextBatch()
	{
		Flush();
		StartBatch();
	}

	//When we create a model on a modelComp using Model::Create() this also gets called.
	void Renderer3D::LoadModel(std::shared_ptr<Model> model, uint32_t entityID)
	{
		SOL_PROFILE_FUNCTION();
		auto& meshes = model->GetMeshes();

		for (size_t i = 0; i < meshes.size(); i++)
		{
			LoadMesh(meshes[i],model->GetName(),entityID);
		}
	}

	void Renderer3D::LoadMesh(const std::shared_ptr<Mesh>& mesh,const std::string& modelName, uint32_t entityID)
	{
		SOL_PROFILE_FUNCTION();
		auto name = mesh->Name + modelName;

		//TODO check if the mesh already has a mesh render data, then add an instance to it and exit.
		//TODO increase instances with each model loaded

		if (s_3DData.MeshDataCollections.Exists(name)) 
		{
			auto& meshRenderData = s_3DData.MeshDataCollections.Get(name);
			meshRenderData.m_Instances.push_back(entityID, InstanceData());
			meshRenderData.m_InstanceBuffer->SetData(meshRenderData.m_Instances.data(), meshRenderData.m_Instances.size() * sizeof(InstanceData));
			meshRenderData.m_VertexArray->SetInstanceBuffer(meshRenderData.m_InstanceBuffer);
			meshRenderData.m_ContainedEntityIds.push_back(entityID);
			return;
		}

		uint32_t maxVerts = s_3DData.MaxMeshes * mesh->Vertices.size();
		uint32_t maxIndices = s_3DData.MaxMeshes * mesh->Indices.size();

		MeshRenderData meshData;
		meshData.m_Mesh = mesh;
		meshData.MaxIndicies = maxIndices;
		meshData.MaxVerts = maxVerts;
		meshData.IndexOffset = mesh->Indices.size();
		meshData.m_VertexArray = VertexArray::Create();
		meshData.m_VertexBuffer= VertexBuffer::Create(mesh->Vertices.size() * sizeof(Vertex));

		//TODO instance data not updating in shader probably has something to do with it not being set as a part of the layout
		meshData.m_VertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position"     },
			{ ShaderDataType::Float3, "a_Normal"     },
			{ ShaderDataType::Float2, "a_TexCoord"     },
			{ ShaderDataType::Float4, "a_Color"     },
			{ ShaderDataType::Int, "a_EntityID"     }
			});

		//TODO with InstanceRendering we should only need one vertex buffer, 
		// the InstanceBuffer should be what we have a collection of. 
		// Make sure we update vertex array to be able to hold and interpret multiple instance buffers and only one vertex buffer.
		meshData.m_VertexArray->AddVertexBuffer(meshData.m_VertexBuffer);

		meshData.VertexBufferBase = new Vertex[maxVerts];
		meshData.InstanceBufferBase = new InstanceData[s_3DData.MaxMeshes];

		//TODO so this might actually be a problem the gpu might need multiple index buffers even if the idicies are the same
		meshData.m_IndexBuffer = IndexBuffer::Create(mesh->Indices.data(), mesh->Indices.size());
		meshData.m_VertexArray->SetIndexBuffer(meshData.m_IndexBuffer);


		//TODO should use missing texture to color 3d mesh
		//TODO I think the reason nothing shows up in the view port is becasuse we don't set the model unifrom in teh default shader
		//TODO make new default shader for use with 3d meshes, base it of of quad shader
		meshData.Shader = Shader::Create("cube.vert", "cube.frag", "Default");//TODO replace this with something we set in the material


		std::vector<VertexAttributeSpecs> vertAtribSpecs =
		{
			VertexAttributeSpecs(3,offsetof(InstanceData, MeshPosition))
		};

		meshData.m_Instances.push_back(entityID, InstanceData());

		auto instanceStride = sizeof(InstanceData);
		//TODO make it so we don't manually have to set the layoutOffset
		//TODO Also make sure that the instance data is able to be modifed and be reflected in the rendering and that it is actually used.
		meshData.m_InstanceBuffer = InstanceBuffer::Create(
			s_3DData.MaxMeshes * instanceStride,
			instanceStride, 
			vertAtribSpecs,
			5 //because we have five elements in the shader layout already
			);
		meshData.m_InstanceBuffer->SetData(meshData.m_Instances.data(), meshData.m_Instances.size() * sizeof(InstanceData));
		meshData.m_VertexArray->SetInstanceBuffer(meshData.m_InstanceBuffer);
		meshData.m_ContainedEntityIds.push_back(entityID);

		s_3DData.MeshDataCollections.push_back(name, meshData);
	}

	void Renderer3D::DrawInstances()
	{
		SOL_PROFILE_FUNCTION();
		for (auto& renderData :s_3DData.MeshDataCollections)
		{
			SOL_PROFILE_FUNCTION();

			//TODO make sure that this gives us the correct buffers
			
			auto& mesh = renderData.m_Mesh;
			uint32_t vertexCount = mesh->Vertices.size();

			if (renderData.IndexCount >= renderData.MaxIndicies)
				NextBatch();

			//renderData.m_InstanceBuffer->SetData(renderData.Instances.data(), renderData.Instances.size() * sizeof(InstanceData));

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
			
			for (auto& instanceData :renderData.m_Instances)
			{
				renderData.InstanceBufferPtr->MeshPosition = instanceData.MeshPosition;
				renderData.InstanceBufferPtr++;
			}

			renderData.IndexCount += mesh->Indices.size();

			//Mesh count needs to be reset
			//s_3DData.Stats.MeshCount++;
		}
	
	}

	void Renderer3D::DrawModel(std::shared_ptr<Model> model, const glm::mat4& transform)
	{
		SOL_PROFILE_FUNCTION();
		auto& meshes = model->GetMeshes();

		for (size_t i = 0; i < meshes.size(); i++)
		{
			DrawMesh(model->GetName(), meshes[i], transform);
		}
	}
	//This needs to be changed to reflect instanced rendering instead of teh old batch rendering,
	//Meaning instead of setting color and stuff here we should be setting the instancedBuffer values
	void Renderer3D::DrawMesh(const std::string& modelName,const std::shared_ptr<Mesh>& mesh, const glm::mat4& transform)
	{
		SOL_PROFILE_FUNCTION();

		//TODO make sure that this gives us the correct buffers
		auto name = mesh->Name + modelName;
		auto& renderData = s_3DData.MeshDataCollections.Get(name);

		uint32_t vertexCount = mesh->Vertices.size();
		
		if (renderData.IndexCount >= renderData.MaxIndicies)
			NextBatch();

		//renderData.m_InstanceBuffer->SetData(renderData.Instances.data(), renderData.Instances.size() * sizeof(InstanceData));

		for (size_t i = 0; i < vertexCount; i++)
		{
			auto& vert = mesh->Vertices[i];
			renderData.VertexBufferPtr->Position = transform * glm::vec4(vert.Position,0.f);
			renderData.VertexBufferPtr->Normal = vert.Normal;
			renderData.VertexBufferPtr->TexCoords = vert.TexCoords;
			renderData.VertexBufferPtr->Color = glm::vec4(1.f, 0.f, 0.f, 1.f);
			renderData.VertexBufferPtr->EntityID = 0;
			renderData.VertexBufferPtr++;
		}

		renderData.IndexCount += mesh->Indices.size();

		//Mesh count needs to be reset
		//s_3DData.Stats.MeshCount++;
	}

	void Renderer3D::UpdateInstanceData(uint32_t entityID, const InstanceData& instanceData)
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
			collection.m_Instances[entityID] = instanceData;
			collection.m_InstanceBuffer->SetData(collection.m_Instances.data(), collection.m_Instances.size() * sizeof(InstanceData));
			collection.m_VertexArray->SetInstanceBuffer(collection.m_InstanceBuffer);
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
