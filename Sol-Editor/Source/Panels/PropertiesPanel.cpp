#include "PropertiesPanel.h"

#include "Sol/Scene/Components.h"

#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>
#include <imgui_internal.h>

namespace Sol
{

	static void DrawDragFloatWithReset(const std::string& label, ImFont* bold, ImVec2& buttonSize, float& value, std::array<ImVec4, 2> colors, float resetValue = 0.0f, float stepSize = 0.1f, float min = 0.0f, float max = 0.0f)
	{

		ImGui::PushStyleColor(ImGuiCol_Button, colors[0]);
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, colors[1]);
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, colors[0]);

		ImGui::PushFont(bold);
		if (ImGui::Button(label.c_str(), buttonSize))
			value = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		std::string hash = "##";
		ImGui::DragFloat((hash + label).c_str(), &value, stepSize, min, max, "%.2f");
		ImGui::PopItemWidth();
	}

	static void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float stepSize = 0.1f, float min = 0.0f, float max = 0.0f, std::array<std::string, 3> buttonLabels = { "X","Y","Z" }, float columnWidth = 80.f)
	{
		ImGuiIO& io = ImGui::GetIO();
		auto bold = io.Fonts->Fonts[1];

		ImGui::PushID(label.c_str());
		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0,0 });
		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.f;

		ImVec2 buttonSize = { lineHeight - 8.f, lineHeight };

		{
			std::array<ImVec4, 2> colors = { ImVec4{0.9f, 0.2f, 0.15f, 1.f}, ImVec4{1.f, 0.2f, 0.2f, 1.f} };
			DrawDragFloatWithReset(buttonLabels[0], bold, buttonSize, values.x, colors, resetValue, stepSize, min, max);
			ImGui::SameLine();
		}

		{
			std::array<ImVec4, 2> colors = { ImVec4{ 0.2f,0.7f,0.15f,1.f }, { 0.2f,0.8f,0.2f,1.f } };
			DrawDragFloatWithReset(buttonLabels[1], bold, buttonSize, values.y, colors, resetValue, stepSize, min, max);
			ImGui::SameLine();
		}

		{
			std::array<ImVec4, 2> colors = { ImVec4{ 0.15f,0.2f,0.8f,1.f }, ImVec4{ 0.2f,0.2f,1.f,1.f } };
			DrawDragFloatWithReset(buttonLabels[2], bold, buttonSize, values.z, colors, resetValue, stepSize, min, max);
		}

		ImGui::PopStyleVar();
		ImGui::Columns(1);
		ImGui::PopID();
	}

	static void DrawVec4Control(const std::string& label, glm::vec4& values, float resetValue = 0.0f, float stepSize = 0.1f, float min = 0.0f, float max = 0.0f, std::array<std::string, 4> buttonLabels = { "X","Y","Z","W" }, float columnWidth = 80.f)
	{
		ImGuiIO& io = ImGui::GetIO();
		auto bold = io.Fonts->Fonts[1];

		ImGui::PushID(label.c_str());
		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(4, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0,0 });
		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.f;

		ImVec2 buttonSize = { lineHeight - 8.f, lineHeight };

		{
			std::array<ImVec4, 2> colors = { ImVec4{0.9f, 0.2f, 0.15f, 1.f}, ImVec4{1.f, 0.2f, 0.2f, 1.f} };
			DrawDragFloatWithReset(buttonLabels[0], bold, buttonSize, values.x, colors, resetValue, stepSize, min, max);
			ImGui::SameLine();
		}

		{
			std::array<ImVec4, 2> colors = { ImVec4{ 0.2f,0.7f,0.15f,1.f }, { 0.2f,0.8f,0.2f,1.f } };
			DrawDragFloatWithReset(buttonLabels[1], bold, buttonSize, values.y, colors, resetValue, stepSize, min, max);
			ImGui::SameLine();
		}

		{
			std::array<ImVec4, 2> colors = { ImVec4{ 0.15f,0.2f,0.8f,1.f }, ImVec4{ 0.2f,0.2f,1.f,1.f } };
			DrawDragFloatWithReset(buttonLabels[2], bold, buttonSize, values.z, colors, resetValue, stepSize, min, max);
			ImGui::SameLine();
		}

		{
			std::array<ImVec4, 2> colors = { ImVec4{ 0.4f,0.4f,0.4f,1.f }, ImVec4{ 0.6f,0.6f,0.6f,1.f } };
			DrawDragFloatWithReset(buttonLabels[3], bold, buttonSize, values.w, colors, resetValue, stepSize, min, max);
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
