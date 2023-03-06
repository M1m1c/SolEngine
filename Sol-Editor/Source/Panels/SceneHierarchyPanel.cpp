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

				Entity entity{ entityID, m_CurrentScene.get() };
				DrawEntityNode(entity);

			});

		ImGui::End();
	}

	void SceneHierarchyPanel::DrawEntityNode(Entity entity)
	{
		auto name = entity.GetComponent<NameComp>().GetName();
		ImGuiTreeNodeFlags flags = 
			(m_CurrentSelection == entity ? ImGuiTreeNodeFlags_Selected : 0) |
			ImGuiTreeNodeFlags_OpenOnArrow;

		bool opened= ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, name.c_str());
		if (ImGui::IsItemClicked())
		{
			m_CurrentSelection = entity;
		}

		if (opened)
		{
			ImGui::TreePop();
		}
	}

}

