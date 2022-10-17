#include "solpch.h"
#include "Scene.h"

namespace Sol
{
	Scene::Scene()
	{
		m_ECSAdmin.Init();
		
	}

	Scene::~Scene()
	{
	}

	void Scene::OnUpdate(TimeStep deltaTime)
	{
	}

	/*Entity& Scene::CreateEntity()
	{
		return m_ECSAdmin.CreateEntity();
	}*/

}