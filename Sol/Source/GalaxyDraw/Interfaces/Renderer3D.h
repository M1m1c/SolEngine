#pragma once

#include "OrthoCamera.h"
#include "Texture.h"
#include "GalaxyDraw/Camera.h"
//#include "Hazel/Renderer/EditorCamera.h"
#include "Sol/Scene/Components.h"

namespace GalaxyDraw {

	//TODO maybe instance data should actually hold refernces to the transforms,
	// so that when we render a mesh we simply read the reference and forward that info to the draw elements instanced
	struct InstanceData
	{
		glm::vec3 MeshPosition;
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
		static void UpdateInstanceData(uint32_t entityID, const InstanceData& instanceData);

		static void LoadModel(std::shared_ptr<Model> model, uint32_t entityID);
		static void LoadMesh(const Mesh& mesh, const std::string& modelName, uint32_t entityID);

		static void DrawModel(std::shared_ptr<Model> model, const glm::mat4& transform);
		static void DrawMesh(const std::string& modelName,const Mesh& mesh, const glm::mat4& transform);

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
		static void StartBatch();
		static void Flush();
		static void NextBatch();
	};
}