#include "PropertiesPanel.h"

#include "Sol/Scene/Components.h"

#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>
#include <imgui_internal.h>

namespace Sol {

	static void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 80.f)
	{
		ImGui::PushID(label.c_str());
		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0,0 });
		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.f;

		ImVec2 buttonSize = { lineHeight + 3.f, lineHeight };

		{
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.9f,0.2f,0.15f,1.f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 1.f,0.2f,0.2f,1.f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.9f,0.2f,0.15f,1.f });

			if (ImGui::Button("X", buttonSize))
				values.x = resetValue;
			ImGui::PopStyleColor(3);

			ImGui::SameLine();
			ImGui::DragFloat("##X", &values.x, 0.1f, 0.f, 0.f, "%.2f");
			ImGui::PopItemWidth();
			ImGui::SameLine();
		}

		{
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f,0.7f,0.15f,1.f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f,0.8f,0.2f,1.f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f,0.7f,0.15f,1.f });

			if (ImGui::Button("Y", buttonSize))
				values.y = resetValue;
			ImGui::PopStyleColor(3);

			ImGui::SameLine();
			ImGui::DragFloat("##Y", &values.y, 0.1f, 0.f, 0.f, "%.2f");
			ImGui::PopItemWidth();
			ImGui::SameLine();
		}

		{
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.15f,0.2f,0.8f,1.f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f,0.2f,1.f,1.f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.15f,0.2f,0.8f,1.f });

			if (ImGui::Button("Z", buttonSize))
				values.z = resetValue;
			ImGui::PopStyleColor(3);

			ImGui::SameLine();
			ImGui::DragFloat("##Z", &values.z, 0.1f, 0.f, 0.f, "%.2f");
			ImGui::PopItemWidth();
		}

		ImGui::PopStyleVar();
		ImGui::Columns(1);
		ImGui::PopID();
	}

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
				if (ImGui::MenuItem("Model Component")) 
				{
					//TODO provide a path to model file
					m_CurrentSelection.AddComponent<ModelComp>("assets/models/cube.fbx", m_CurrentSelection.GetID());
					m_CurrentSelection.AddComponent<MaterialComp>();
					ImGui::CloseCurrentPopup();
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
			ImGui::SameLine(ImGui::GetWindowWidth()-25.f);

			if (ImGui::Button("+", ImVec2{20,20})) {
				ImGui::OpenPopup("ComponentSettings");
			}
			ImGui::PopStyleVar();

			bool removeComponent = false;
			if(ImGui::BeginPopup("ComponentSettings"))
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
				auto& material = entity.GetComponent<MaterialComp>();

				ImGui::Columns(2);
				ImGui::SetColumnWidth(0, 100.f);
				ImGui::Text("Color");
				ImGui::NextColumn();
				ImGui::DragFloat4("##MaterialColor", glm::value_ptr(material.Color), 0.01f,0.0f,1.f);
				ImGui::Columns(1);
				ImGui::TreePop();
			}
		}
	}
}
