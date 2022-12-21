#include "solpch.h"
#include "Renderer3D.h"

#include "VertexArray.h"
#include "Shader.h"
#include "UniformBuffer.h"
#include "RenderCommand.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace GalaxyDraw 
{

	//TODO make sure that only one of these exist per unique mesh, if more meshes of the same are loaded add their data to the buffers
	// might want to store a map of string to id, pass in the name of the mesh file and get the mes render data name.
	struct MeshRenderData 
	{
		std::string Name;
		std::shared_ptr<VertexArray> VertexArray;
		std::shared_ptr<VertexBuffer> VertexBuffer;
		std::shared_ptr<Shader> Shader;

		uint32_t IndexCount = 0;
		uint32_t IndexOffset = 0;
		Vertex* VertexBufferBase = nullptr;
		Vertex* VertexBufferPtr = nullptr;
	};

	struct Renderer3DData
	{
		static const uint32_t MaxMeshes = 20000;
		static const uint32_t MaxTextureSlots = 32; // TODO: RenderCaps

		std::unordered_map<std::string, MeshRenderData> MeshDataCollection;
		std::shared_ptr<Texture2D> MissingTexture;

		Renderer3D::Statistics Stats;

		struct CameraData
		{
			glm::mat4 ViewProjection;
		};
		CameraData CameraBuffer;
		std::shared_ptr<UniformBuffer> CameraUniformBuffer;
	};

	static Renderer3DData s_Data;

	void GalaxyDraw::Renderer3D::Init()
	{
		SOL_PROFILE_FUNCTION();

		s_Data.MissingTexture = Texture2D::Create(1, 1);
		uint32_t missingTextureData = 0xff00ff;
		s_Data.MissingTexture->SetData(&missingTextureData, sizeof(uint32_t));
	}

	void GalaxyDraw::Renderer3D::Shutdown()
	{
		SOL_PROFILE_FUNCTION();
		for (size_t i = 0; i < s_Data.MeshDataCollection.size(); i++)
		{
			delete[] s_Data.MeshDataCollection[i].VertexBufferBase;
		}
		
	}

	void GalaxyDraw::Renderer3D::BeginScene(const glm::mat4& projection, const glm::mat4& transform)
	{
		SOL_PROFILE_FUNCTION();

		s_Data.CameraBuffer.ViewProjection = projection * glm::inverse(transform);
		s_Data.CameraUniformBuffer->SetData(&s_Data.CameraBuffer, sizeof(Renderer3DData::CameraData));

		StartBatch();
	}

	void GalaxyDraw::Renderer3D::BeginScene(const OrthoCamera& camera)
	{
		SOL_PROFILE_FUNCTION();

		s_Data.CameraBuffer.ViewProjection = camera.GetViewProjectionMatrix();
		s_Data.CameraUniformBuffer->SetData(&s_Data.CameraBuffer, sizeof(Renderer3DData::CameraData));

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
		for (size_t i = 0; i < s_Data.MeshDataCollection.size(); i++)
		{
			auto& meshData = s_Data.MeshDataCollection[i];

			meshData.IndexCount = 0;
			meshData.VertexBufferPtr = meshData.VertexBufferBase;
			//s_Data.TextureSlotIndex = 1;
		}
	}

	void GalaxyDraw::Renderer3D::Flush()
	{
		for (size_t i = 0; i < s_Data.MeshDataCollection.size(); i++)
		{
			auto& meshData = s_Data.MeshDataCollection[i];
			if (meshData.IndexCount)
			{
				uint32_t dataSize = (uint32_t)((uint8_t*)meshData.VertexBufferPtr - (uint8_t*)meshData.VertexBufferBase);
				meshData.VertexBuffer->SetData(meshData.VertexBufferBase, dataSize);

				//// Bind textures
				//for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
				//	s_Data.TextureSlots[i]->Bind(i);

				meshData.Shader->Bind();
				RenderCommand::DrawIndexed(meshData.VertexArray, meshData.IndexCount);
				s_Data.Stats.DrawCalls++;
			}
		}
	}

	void Renderer3D::NextBatch()
	{
		Flush();
		StartBatch();
	}

	//When we create a model on a modelComp using Model::Create() this also gets called.
	void Renderer3D::LoadModel(std::shared_ptr<Model> model)
	{
		SOL_PROFILE_FUNCTION();
		auto& meshes = model->GetMeshes();

		for (size_t i = 0; i < meshes.size(); i++)
		{
			LoadMesh(meshes[i],model->GetName());
		}
	}

	//TODO Need to set the name/id so we don't create duplicate MeshDataCollecitons.
	void Renderer3D::LoadMesh(const Mesh& mesh,const std::string& modelName)
	{
		SOL_PROFILE_FUNCTION();
		auto name = mesh.Name + modelName;

		uint32_t maxVerts = s_Data.MaxMeshes * mesh.Vertices.size();
		uint32_t maxIndices = s_Data.MaxMeshes * mesh.Indices.size();

		MeshRenderData meshData;
		meshData.IndexOffset = mesh.Indices.size();
		meshData.VertexArray = VertexArray::Create();
		meshData.VertexBuffer= VertexBuffer::Create(maxVerts * sizeof(Vertex));

		meshData.VertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position"     },
			{ ShaderDataType::Float4, "a_Color"        },
			{ ShaderDataType::Float2, "a_TexCoord"     },
			{ ShaderDataType::Float,  "a_TexIndex"     },
			{ ShaderDataType::Float,  "a_TilingFactor" },
			{ ShaderDataType::Int,    "a_EntityID"     }
			});
		meshData.VertexArray->AddVertexBuffer(meshData.VertexBuffer);

		meshData.VertexBufferBase = new Vertex[maxVerts];
		std::shared_ptr<IndexBuffer> indexBuffer = IndexBuffer::Create(mesh.Indices.data(), maxIndices);
		meshData.VertexArray->SetIndexBuffer(indexBuffer);

		meshData.Shader = Shader::Create("quad.vert", "quad.frag", "Quad2");//TODO replace this with something we set in the material

		s_Data.MeshDataCollection.insert({ name, meshData });
		//s_Data.MeshDataCollection.push_back(meshData);
	}

	void Renderer3D::DrawModel(std::shared_ptr<Model> model, const glm::mat4& transform, int entityID)
	{
		SOL_PROFILE_FUNCTION();
		auto& meshes = model->GetMeshes();

		//TODO get the mesh render data corresponding to mesh and pass that along

		for (size_t i = 0; i < meshes.size(); i++)
		{
			DrawMesh(model->GetName(), meshes[i], transform, entityID);
		}
	}

	void Renderer3D::DrawMesh(const std::string& modelName,const Mesh& mesh, const glm::mat4& transform, int entityID)
	{
		SOL_PROFILE_FUNCTION();

		//TODO make sure that this gives us the correct buffers
		auto name = mesh.Name + modelName;
		auto& renderData = s_Data.MeshDataCollection[name];

		uint32_t vertexCount = mesh.Vertices.size();

		/*if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices)
			NextBatch();

		for (size_t i = 0; i < quadVertexCount; i++)
		{
			s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[i];
			s_Data.QuadVertexBufferPtr->Color = color;
			s_Data.QuadVertexBufferPtr->TexCoord = textureCoords[i];
			s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_Data.QuadVertexBufferPtr->EntityID = entityID;
			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.QuadIndexCount += 6;

		s_Data.Stats.QuadCount++;*/
	}

	void Renderer3D::ResetStats()
	{
		memset(&s_Data.Stats, 0, sizeof(Statistics));
	}

	Renderer3D::Statistics Renderer3D::GetStats()
	{
		return s_Data.Stats;
	}

}
