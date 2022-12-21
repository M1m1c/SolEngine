#pragma once

#include "OrthoCamera.h"
#include "Texture.h"
#include "GalaxyDraw/Camera.h"
//#include "Hazel/Renderer/EditorCamera.h"
#include "Sol/Scene/Components.h"

namespace GalaxyDraw {

	class Renderer3D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const glm::mat4& projection, const glm::mat4& transform);
		//static void BeginScene(const EditorCamera& camera);
		static void BeginScene(const OrthoCamera& camera); // TODO: Remove
		static void EndScene();


		static void LoadModel(std::shared_ptr<Model> model);
		static void LoadMesh(const Mesh& mesh, const std::string& modelName);

		static void DrawModel(std::shared_ptr<Model> model, const glm::mat4& transform, int entityID);
		static void DrawMesh(const std::string& modelName,const Mesh& mesh, const glm::mat4& transform, int entityID);

		struct Statistics
		{
			uint32_t DrawCalls = 0;
			uint32_t QuadCount = 0;

			uint32_t GetTotalVertexCount() const { return QuadCount * 4; }
			uint32_t GetTotalIndexCount() const { return QuadCount * 6; }
		};
		static void ResetStats();
		static Statistics GetStats();
	private:
		static void StartBatch();
		static void Flush();
		static void NextBatch();
	};
}