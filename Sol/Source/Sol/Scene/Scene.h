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

		//Creates an entity with a TransformComp and a TagComp in the scene, returns created entity.
		Entity CreateEntity(const std::string& name = std::string());

	private:

		entt::registry m_Registry;

		friend class Entity;
	};

}

