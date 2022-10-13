#pragma once
#include "Sol/ECS/ECS_Admin.h"

namespace Sol 
{
	class Scene
	{
	public:
		Scene();
		~Scene();

	private:
		ECS_Admin m_ECSAdmin;
	};

}

