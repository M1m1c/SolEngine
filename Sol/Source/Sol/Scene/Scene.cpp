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
		auto group = m_Registry.group<TransformComp>(entt::get<SpriteRendererComp>);
		for (auto entity : group)
		{
			auto& [transform, sprite] = group.get<TransformComp, SpriteRendererComp>(entity);

			GD_Renderer::DrawQuad(transform,sprite.Color);
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