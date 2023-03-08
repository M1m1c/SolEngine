#include "PropertiesPanel.h"

#include "PanelUtils.h"
#include "Sol/Scene/Components.h"

#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>
#include <imgui_internal.h>

namespace Sol
{

	void PropertiesPanel::OnImGuiRender()
	{
		ImGui::Begin("Properties");
		if (m_CurrentSelection)
		{
			DrawComponentNodes(m_CurrentSelection);

			if (ImGui::Button("Add Component"))
			{
				ImGui::OpenPopup("AddComponent");
			}

			if (ImGui::BeginPopup("AddComponent"))
			{
				if (!m_CurrentSelection.HasComponent<ModelComp>())
				{
					if (ImGui::MenuItem("Model Component"))
					{
						//TODO provide a path to model file
						m_CurrentSelection.AddComponent<ModelComp>("assets/models/cube.fbx", m_CurrentSelection.GetID());
						if (!m_CurrentSelection.HasComponent<MaterialComp>())
						{
							m_CurrentSelection.AddComponent<MaterialComp>();
						}
						ImGui::CloseCurrentPopup();
					}
				}

				ImGui::EndPopup();
			}
		}
		ImGui::End();

	}
	void PropertiesPanel::DrawComponentNodes(Entity entity)
	{
		if (entity.HasComponent<NameComp>())
		{
			auto& name = entity.GetComponent<NameComp>().GetName();

			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, sizeof(buffer), name.c_str());

			ImGui::Columns(2);
			ImGui::SetColumnWidth(0, 100.f);
			ImGui::Text("Name");
			ImGui::NextColumn();
			if (ImGui::InputText("##Name", buffer, sizeof(buffer))) {
				name = std::string(buffer);
			}
			ImGui::Columns(1);
		}

		const ImGuiTreeNodeFlags treeNodeFlags = 
			ImGuiTreeNodeFlags_DefaultOpen |
			ImGuiTreeNodeFlags_AllowItemOverlap |
			ImGuiTreeNodeFlags_SpanAvailWidth;

		DrawComponent<TransformComp>("Transform", entity, treeNodeFlags, [](TransformComp& component) {

			auto& position = component.Position;
			DrawVec3Control("Position", position);

			auto& rotation = component.Rotation;
			DrawVec3Control("Rotation", rotation);

			auto& scale = component.Scale;
			DrawVec3Control("Scale", scale, 1.f);

			}, false);

		DrawComponent<ModelComp>("Model", entity, treeNodeFlags, [](ModelComp& component) {
			ImGui::Columns(2);
			ImGui::SetColumnWidth(0, 80.f);
			ImGui::Text("Name");
			ImGui::NextColumn();
			ImGui::Text(component.Model->GetName().c_str());
			ImGui::Columns(1);
			});

		DrawComponent<MaterialComp>("Material", entity, treeNodeFlags, [](MaterialComp& component) {
			auto& color = component.Color;
			DrawVec4Control("Color", color, 1.f, 0.01f, 0.f, 1.f, { "R","G","B","A" });
			});
	}
}
