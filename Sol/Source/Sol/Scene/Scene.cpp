#include "solpch.h"
#include "Scene.h"
#include "Entity.h"
#include "Sol/Scene/Components.h"
#include "GalaxyDraw/GalaxyDraw.h"

namespace Sol
{
	Scene::Scene()
	{
	}

	Scene::~Scene()
	{
	}

	void Scene::OnUpdateRuntime(TimeStep deltaTime)
	{

		GD_Camera* mainCamera = nullptr;
		glm::mat4 cameraViewMat = glm::mat4(1.f);
		{
			auto view = m_Registry.view<TransformComp, CameraComp>();
			for (auto entity : view)
			{
				auto& [transform, camera] = view.get<TransformComp, CameraComp>(entity);

				if (camera.IsPirmary)
				{
					mainCamera = &camera.Camera;
					cameraViewMat = transform.GetViewMatrix();
					break;
				}
			}
		}



		if (mainCamera)
		{
			glm::mat4 projectionMat = mainCamera->GetProjection();
			GD_Renderer3D::BeginScene(projectionMat, cameraViewMat);

			auto group3D = m_Registry.group<ModelComp>(entt::get<TransformComp, MaterialComp>);
			for (auto entity : group3D)
			{
				auto& [transform, material] = group3D.get<TransformComp, MaterialComp>(entity);

				auto temp = GD_::InstanceData();

				temp.m_EntityTransform = transform;
				//temp.m_MeshPosition = transform.Position;
				temp.m_MeshColor = material.Color;

				GD_Renderer3D::UpdateInstanceData(entity, temp);
				//GD_Renderer3D::DrawModel(model.Model, transform);
			}
			GD_Renderer3D::DrawInstances();

			GD_Renderer3D::EndScene();


			GD_Renderer2D::BeginScene(projectionMat, cameraViewMat);

			auto group2D = m_Registry.group<SpriteRendererComp>(entt::get<TransformComp>);
			for (auto entity : group2D)
			{
				auto& [sprite, transform] = group2D.get<SpriteRendererComp, TransformComp>(entity);

				GD_Renderer2D::DrawQuad(transform, sprite.Color);
			}

			GD_Renderer2D::EndScene();
		}
	}

	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;

		auto view = m_Registry.view<CameraComp>();
		for (auto entity : view)
		{
			auto& camComp = view.get<CameraComp>(entity);

			if (!camComp.IsFixedAspectRatio)
			{
				camComp.Camera.SetViewportSize(width, height);
			}

		}
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		auto entity = Entity{ m_Registry.create(), this };
		entity.AddComponent<TransformComp>();
		entity.AddComponent<NameComp>(name == "" ? "Entity" : name);
		return entity;
	}

	void Scene::DestroyEntity(EntityID entityID)
	{
		//TODO it needs to call destroy on all the components too
		if (m_Registry.any_of<ModelComp>(entityID))
		{
			auto& modelManager = GD_ModelManager::GetInstance();
			auto& modelComp = m_Registry.get<ModelComp>(entityID);

			GD_Renderer3D::DiscardMeshInstances(entityID, modelManager.GetModel(modelComp.ModelPath));
			modelManager.DiscardModelInstance(modelComp.ModelPath);
		}
		m_Registry.destroy(entityID);
	}

	void Scene::DestroyAllEntities()
	{
		m_Registry.each([&](EntityID entityID) {
			DestroyEntity(entityID);
			});
	}
}