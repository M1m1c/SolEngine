#pragma once

#include "OrthoCamera.h"
#include "GalaxyDraw/Camera.h"
//#include "Hazel/Renderer/EditorCamera.h"

#include "Sol/SolDefines.h"


namespace GalaxyDraw {

	class IModel;
	struct Mesh;
	struct InstanceData;
	struct MaterialData;

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

		//Returns the materialIndex
		static uint32_t UpdateExistingMaterial(const std::string& texturePath, const uint32_t matIndex, const EntityID entityID);
		static uint32_t CreateNewMaterial(const std::string& texturePath, const EntityID entityID);
		static uint32_t SwapMaterial(const uint32_t matIndex, const EntityID entityID);

		
		static uint32_t GetMaterialIndex(EntityID entityID);
		static std::shared_ptr<MaterialData> GetMaterial(uint32_t materialIndex);
		static std::vector<std::shared_ptr<MaterialData>>& GetAllMaterials();

		//TODO create function for handeling when entity is destroyed, needs to remove it self from relevant MeshRenderData m_ContainedEntityIds.
		//TODO crate function for unloading a model and mesh.
		//TODO fix so that when models wiht seperate sub meshes get loadad that each sub mesh instantiates a new entity wiht a transform at that sub meshes location relative to parent model.

		static std::string DiscardEntityRenderData(const EntityID& entityID, bool shouldDiscardMaterial=true,bool shouldDiscardModel=true);


		//draws all instances of meshes
		static void DrawInstances();


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
		static std::shared_ptr<MaterialData> CreateMaterialData(std::string matName, std::pair<std::string, std::string> shaderFiles, std::string shaderName, std::string texturePath);
		static void DiscardMeshInstances(EntityID entityID, std::shared_ptr<MaterialData> matData);
		static void ReloadModel(std::string& modelPath, const EntityID& entityID, const uint32_t& materialIndex);


		//static CreateNewMaterial(const std::string& texturePath);
	};
}