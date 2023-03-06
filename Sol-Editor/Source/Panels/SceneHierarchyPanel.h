#pragma once
#include "Sol/Core/Core.h"
#include "Sol/Core/Log.h"
#include "Sol/Scene/Scene.h"
#include "Sol/Scene/Entity.h"

#include "PropertiesPanel.h"

namespace Sol {
	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const s_ptr<Scene>& scene);
		~SceneHierarchyPanel() = default;

		void SetCurrentScene(const s_ptr<Scene>& scene) { m_CurrentScene = scene; }
		void SetPropertiesPanel(PropertiesPanel* panel) { m_PropertiesPanel = panel; }

		void OnImGuiRender();

	private:
		void DrawEntityNode(Entity entity);

	private:

		s_ptr<Scene> m_CurrentScene;
		Entity m_OldSelection;
		Entity m_CurrentSelection;
		PropertiesPanel* m_PropertiesPanel;
	};
}