#pragma once

#include "OrthoCamera.h"
#include "GalaxyDraw/Camera.h"
//#include "Hazel/Renderer/EditorCamera.h"
#include "Sol/Scene/Components.h"
#include "Sol/SolDefines.h"

namespace GalaxyDraw {

	//!VERY IMPORTANT!
	// The order of initaliseation needs to match the location order in the shader,
	// otherwise the columns will be missaligned.
	struct InstanceData
	{
		InstanceData() :
			m_EntityID(-1),
			m_TextureID(-1),
			m_MeshColor(),
			m_EntityTransform(),
			m_MeshTransform()
			//	m_MeshPosition(),
		{};

		int m_EntityID;
		int m_TextureID;
		glm::vec4 m_MeshColor;
		glm::mat4 m_EntityTransform;
		glm::mat4 m_MeshTransform;
		//glm::vec3 m_MeshPosition;
	};

	class Renderer3D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const glm::mat4& projection, const glm::mat4& transform);
		//static void BeginScene(const EditorCamera& camera);
		static void BeginScene(const OrthoCamera& camera); // TODO: Remove
		static void EndScene();

		//Updates all mesh datacollections instanceData containing entityID
		static void UpdateInstanceData(EntityID entityID, const InstanceData& instanceData);

		static void LoadModel(std::shared_ptr<IModel> model, EntityID entityID, uint32_t materialIndex = 0);
		static void LoadMesh(const std::shared_ptr<Mesh>& mesh, const std::string& modelName, EntityID entityID, uint32_t materialIndex = 0);

		//TODO create function for handeling when entity is destroyed, needs to remove it self from relevant MeshRenderData m_ContainedEntityIds.
		//TODO crate function for unloading a model and mesh.
		//TODO fix so that when models wiht seperate sub meshes get loadad that each sub mesh instantiates a new entity wiht a transform at that sub meshes location relative to parent model.

		//draws all instances of meshes
		static void DrawInstances();

		static void DiscardMeshInstances(EntityID entityID, std::shared_ptr<IModel> model);

		struct Statistics
		{
			uint32_t DrawCalls = 0;
			uint32_t MeshCount = 0;

			uint32_t GetTotalVertexCount() const { return MeshCount * 4; }
			uint32_t GetTotalIndexCount() const { return MeshCount * 6; }
		};
		static void ResetStats();
		static Statistics GetStats();
	private:
		static void Submit();
		static void Flush();
	};
}