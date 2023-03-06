#include "SceneHierarchyPanel.h"

#include "Sol/Scene/Components.h"
#include <imgui.h>

namespace Sol
{
	SceneHierarchyPanel::SceneHierarchyPanel(const s_ptr<Scene>& scene)
	{
		SetCurrentScene(scene);
	}

	SceneHierarchyPanel::~SceneHierarchyPanel()
	{
	}

	void SceneHierarchyPanel::SetCurrentScene(const s_ptr<Scene>& scene)
	{
		m_CurrentScene = scene;
	}

	void SceneHierarchyPanel::OnImGuiRender()
	{
		ImGui::Begin("Scene Hierarchy");
		auto& sceneRegistry = m_CurrentScene->GetRegistry();

		sceneRegistry.each([&](auto entityID)
			{

				Entity entity{ entityID, m_CurrentScene.get()};
				NameComp& name = entity.GetComponent<NameComp>();
				ImGui::Text("%s", name.GetName().c_str());

			});

		ImGui::End();
	}

}

