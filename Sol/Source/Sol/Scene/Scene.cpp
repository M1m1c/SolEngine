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

	void Scene::OnUpdate(TimeStep deltaTime)
	{

		GD_Camera* mainCamera = nullptr;
		glm::mat4* cameraTransform = nullptr;
		{
			auto view = m_Registry.view<TransformComp, CameraComp>();
			for (auto entity : view)
			{
				auto& [transform, camera] = view.get<TransformComp, CameraComp>(entity);

				if (camera.IsPirmary)
				{
					mainCamera = &camera.OrthoCamera;
					cameraTransform = &transform.GetTransformMatrix();
					break;
				}
			}
		}


		if (mainCamera)
		{
			GD_Renderer2D::BeginScene(mainCamera->GetProjection(), *cameraTransform);

			auto group = m_Registry.group<TransformComp>(entt::get<SpriteRendererComp>);
			for (auto entity : group)
			{
				auto& [transform, sprite] = group.get<TransformComp, SpriteRendererComp>(entity);

				GD_Renderer2D::DrawQuad(transform, sprite.Color);
			}
			GD_Renderer2D::EndScene();

			GD_Renderer3D::BeginScene(mainCamera->GetProjection(), *cameraTransform);
			//TODO make sure that the model gets loaded before it is actually used here, and it should only be loaded once
			auto group = m_Registry.group<TransformComp>(entt::get<ModelComp>);
			for (auto entity : group)
			{
				auto& [transform, model] = group.get<TransformComp, ModelComp>(entity);

				//GD_Renderer3D::LoadModel(model.Model);
				GD_Renderer3D::DrawModel(model.Model, transform);
			}

			GD_Renderer3D::EndScene();
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
				camComp.OrthoCamera.SetViewportSize(width, height);
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

}