#pragma once
#include <entt.hpp>
#include <Sol/Core/TimeStep.h>

namespace Sol
{
	class Entity;

	class Scene
	{
	public:
		Scene();
		~Scene();

		void OnUpdate(TimeStep deltaTime);

		void OnViewportResize(uint32_t width, uint32_t height);

		//Creates an entity with a TransformComp and a TagComp in the scene, returns created entity.
		Entity CreateEntity(const std::string& name = std::string());

	private:

		entt::registry m_Registry;

		uint32_t m_ViewportWidth = 0;
		uint32_t m_ViewportHeight = 0;

		friend class Entity;
	};

}

