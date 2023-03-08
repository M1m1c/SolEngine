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

		const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap;

		if (entity.HasComponent<TransformComp>())
		{
			if (ImGui::TreeNodeEx((void*)typeid(TransformComp).hash_code(), treeNodeFlags, "Transform"))
			{
				auto& transform = entity.GetComponent<TransformComp>();

				auto& position = transform.Position;
				DrawVec3Control("Position", position);

				auto& rotation = transform.Rotation;
				DrawVec3Control("Rotation", rotation);

				auto& scale = transform.Scale;
				DrawVec3Control("Scale", scale, 1.f);

				ImGui::TreePop();
			}
		}

		if (entity.HasComponent<ModelComp>())
		{
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4,4 });
			bool open = ImGui::TreeNodeEx((void*)typeid(ModelComp).hash_code(), treeNodeFlags, "Model");
			//TODO this smae line does not end up on the same line, it appears above
			ImGui::SameLine(ImGui::GetWindowWidth() - 25.f);

			if (ImGui::Button("+", ImVec2{ 20,20 })) {
				ImGui::OpenPopup("ComponentSettings");
			}
			ImGui::PopStyleVar();

			bool removeComponent = false;
			if (ImGui::BeginPopup("ComponentSettings"))
			{
				if (ImGui::MenuItem("Remove Compnent"))
				{
					removeComponent = true;
				}
				ImGui::EndPopup();
			}

			if (open)
			{
				auto& model = entity.GetComponent<ModelComp>();

				ImGui::Columns(2);
				ImGui::SetColumnWidth(0, 100.f);
				ImGui::Text("Name");
				ImGui::NextColumn();
				ImGui::Text(model.Model->GetName().c_str());
				ImGui::Columns(1);
				ImGui::TreePop();
			}

			if (removeComponent)
			{
				entity.RemoveComponent<ModelComp>();
			}
		}

		if (entity.HasComponent<MaterialComp>())
		{
			if (ImGui::TreeNodeEx((void*)typeid(MaterialComp).hash_code(), treeNodeFlags, "Material"))
			{
				auto& color = entity.GetComponent<MaterialComp>().Color;
				DrawVec4Control("Color", color, 1.f, 0.01f, 0.f, 1.f, { "R","G","B","A" });

				ImGui::TreePop();
			}
		}
	}
}
