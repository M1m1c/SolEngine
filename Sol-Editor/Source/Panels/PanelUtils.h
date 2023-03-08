#pragma once
#include "Sol/Core/Core.h"
#include "Sol/Core/Log.h"
#include "Sol/Scene/Entity.h"

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

	template<typename T, typename UIFunction>
	static void DrawComponent(const std::string& name, Entity entity, ImGuiTreeNodeFlags treeNodeFlags, UIFunction uiFunction, bool removable = true)
	{
		if (entity.HasComponent<T>())
		{
			auto& component = entity.GetComponent<T>();

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4,4 });
			bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, name.c_str());

			if (removable)
			{
				ImGui::SameLine(ImGui::GetWindowWidth() - 25.f);
				if (ImGui::Button("+", ImVec2{ 20,20 })) {
					ImGui::OpenPopup("ComponentSettings");
				}
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

				uiFunction(component);

				ImGui::TreePop();
			}

			if (removeComponent)
			{
				entity.RemoveComponent<T>();
			}
		}
	}

}