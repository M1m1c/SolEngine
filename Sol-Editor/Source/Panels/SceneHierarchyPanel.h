#pragma once
#include "Sol/Core/Core.h"
#include "Sol/Core/Log.h"
#include "Sol/Scene/Scene.h"
#include "Sol/Scene/Entity.h"

namespace Sol {
	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const s_ptr<Scene>& scene);
		~SceneHierarchyPanel();

		void SetCurrentScene(const s_ptr<Scene>& scene);

		void OnImGuiRender();

	private:
		void DrawEntityNode(Entity entity);

	private:

		s_ptr<Scene> m_CurrentScene;
		Entity m_CurrentSelection;
	};
}