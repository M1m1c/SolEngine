#include "SceneHierarchyPanel.h"

#include "Sol/Scene/Components.h"
#include <imgui.h>

namespace Sol
{
	SceneHierarchyPanel::SceneHierarchyPanel(const s_ptr<Scene>& scene)
	{
		SetCurrentScene(scene);
	}

	
	void SceneHierarchyPanel::OnImGuiRender()
	{
		bool hasActiveScene = m_CurrentScene != nullptr;
		
		ImGui::Begin("Scene Hierarchy");
		if (hasActiveScene) 
		{
			auto& sceneRegistry = m_CurrentScene->GetRegistry();

			sceneRegistry.each([&](auto entityID)
				{

					Entity entity{ entityID, m_CurrentScene.get() };
					bool shouldShow = true;

					auto isInternal = entity.HasComponent<InternalComp>();
					if (isInternal)
					{
						shouldShow = entity.GetComponent<InternalComp>().IsVisible();
					}

					if (shouldShow)
					{
						DrawEntityNode(entity);
					}


				});

			if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
			{
				m_CurrentSelection = {};
			}
		}	
		ImGui::End();

		if (hasActiveScene && m_PropertiesPanel != nullptr && m_CurrentSelection != m_OldSelection)
		{
			m_OldSelection = m_CurrentSelection;
			m_PropertiesPanel->SetCurrentSelection(m_CurrentSelection);
		}
	}

	void SceneHierarchyPanel::DrawEntityNode(Entity entity)
	{
		auto& name = entity.GetComponent<NameComp>().GetName();
		ImGuiTreeNodeFlags flags =
			(m_CurrentSelection == entity ? ImGuiTreeNodeFlags_Selected : 0) |
			ImGuiTreeNodeFlags_OpenOnArrow;

		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, name.c_str());

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

