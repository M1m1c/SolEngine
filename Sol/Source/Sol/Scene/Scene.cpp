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

		GD_RendererCamera* mainCamera = nullptr;
		glm::mat4* cameraTransform = nullptr;
		{
			auto view = m_Registry.view<TransformComp,CameraComp>();
			for (auto entity : view)
			{
				auto& [transform, camera] = view.get<TransformComp, CameraComp>(entity);

				if (camera.IsPirmary) 
				{
					mainCamera = &camera.Camera;
					cameraTransform = &transform.m_Transform;
					break;
				}
			}
		}

		
		if(mainCamera)
		{
			GD_Renderer::BeginScene(mainCamera->GetProjection(), *cameraTransform);

			auto group = m_Registry.group<TransformComp>(entt::get<SpriteRendererComp>);
			for (auto entity : group)
			{
				auto& [transform, sprite] = group.get<TransformComp, SpriteRendererComp>(entity);

				//TODO Render stuff
				GD_Renderer::DrawQuad(transform, sprite.Color);
			}

			GD_Renderer::EndScene();
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

}