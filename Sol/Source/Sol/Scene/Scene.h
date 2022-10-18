#pragma once
#include <entt.hpp>
#include <Sol/Core/TimeStep.h>
//#include "Sol/ECS/Interfaces/Entity.h"

namespace Sol 
{
	class Scene
	{
	public:
		Scene();
		~Scene();

		void OnUpdate(TimeStep deltaTime);

		//TEMP
		entt::registry& Reg() { return m_Registry; }

		entt::entity CreateEntity();

	private:

		entt::registry m_Registry;
	};

}

