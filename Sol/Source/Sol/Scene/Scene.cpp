#include "solpch.h"
#include "Scene.h"
#include "Sol/ECS/Components/Components.h"
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
		auto group = m_Registry.group<TransformComp>(entt::get<SpriteRendererComp>);
		for (auto entity : group)
		{
			auto& [transform, sprite] = group.get<TransformComp, SpriteRendererComp>(entity);

			GD_Renderer::DrawQuad(transform,sprite);
		}
	}

	entt::entity Scene::CreateEntity()
	{
		return m_Registry.create();
	}

}