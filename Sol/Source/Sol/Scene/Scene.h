#pragma once
//#include "Sol/ECS/ECS_Admin.h"
#include <Sol/Core/TimeStep.h>

namespace Sol 
{
	class Scene
	{
	public:
		Scene();
		~Scene();

		void OnUpdate(TimeStep deltaTime);

		//Entity& CreateEntity();

	private:
		//ECS_Admin m_ECSAdmin;
	};

}

