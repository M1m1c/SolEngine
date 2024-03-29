#include "PropertiesPanel.h"

#include "PanelUtils.h"
#include "Sol/Scene/Scene.h"
#include "Sol/Scene/Components.h"
#include "Sol/Utils/PlatformUtils.h"

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

				if (!m_CurrentSelection.HasComponent<DirectionalLightComp>())
				{
					if (ImGui::MenuItem("Directional Light Component"))
					{
						//TODO provide a path to model file
						m_CurrentSelection.AddComponent<DirectionalLightComp>();
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


		DrawComponent<TransformComp>("Transform", entity, [](TransformComp& component) {

			auto& position = component.Position;
			DrawVec3Control("Position", position);

			auto& rotation = component.Rotation;
			DrawVec3Control("Rotation", rotation);

			auto& scale = component.Scale;
			DrawVec3Control("Scale", scale, 1.f);

			}, false);

		DrawComponent<DirectionalLightComp>("DirectionalLight", entity, [](DirectionalLightComp& component) {
			ImGui::Text("Directional Light");
			}, false);

		auto entityID = entity.GetID();
		DrawComponent<ModelComp>("Model", entity, [&](ModelComp& component) {
			ImGui::Columns(2);
			ImGui::SetColumnWidth(0, 80.f);
			ImGui::Text("Model");
			ImGui::NextColumn();

			if (ImGui::Button(component.ModelPath.c_str())) {
				std::string filePath = FileDialogs::OpenFile("fbx (*.fbx)\0*.fbx\0");
				if (!filePath.empty())
				{
					auto matIndex = GD_Renderer3D::GetMaterialIndex(entity);
					auto cleanPath = CleanUpFilePath(filePath);

					//TODO figure out a cleaner way to do this, since we now do this in the scene as well
					if (component.ModelPath != cleanPath)
					{
						GD_Renderer3D::DiscardEntityRenderData(entityID, false);
					}

					component = ModelComp(cleanPath, entityID, matIndex);

				}
			}
			ImGui::Columns(1);
			});

		DrawComponent<MaterialComp>("Material", entity, [&](MaterialComp& component) {

			auto& name = component.GetMaterialName();
			auto matIndex = component.GetMaterialIndex();
			auto entityID = entity.GetID();

			bool readOnly = matIndex == 0;

			ImGui::Columns(2);
			ImGui::SetColumnWidth(0, 80.f);
			ImGui::Text("Name");
			ImGui::NextColumn();

			if (readOnly)
			{
				ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
				ImGui::TextDisabled(name.c_str());
				ImGui::PopStyleVar();
			}
			else
			{
				char buffer[256];
				memset(buffer, 0, sizeof(buffer));
				strcpy_s(buffer, sizeof(buffer), name.c_str());

				if (ImGui::InputText("##MaterialName", buffer, sizeof(buffer)))
				{
					name = std::string(buffer);
				}
			}

			std::vector<std::pair<std::string, std::function<void()>>> buttons;
			std::vector<std::pair<std::string, std::function<void()>>> deleteActions;
			buttons.push_back({ "Create New Material", [&component,entityID]() {
				auto props = component.Properties;
				std::string defaultTexture = "";
				component = MaterialComp(defaultTexture, entityID);
				component.Properties = props;
			} });

			auto& materials = GD_Renderer3D::GetAllMaterials();
			for (size_t i = 0; i < materials.size(); i++)
			{
				if (i == matIndex) { continue; }
				auto& mat = materials[i];
				buttons.push_back({ mat->Name, [&component,i,entityID]() {
					auto props = component.Properties;
					component = MaterialComp(i, entityID);
					component.Properties = props;
				} });

				if (i != 0)
				{
					auto scene = m_CurrentScene;
					deleteActions.push_back({ mat->Name, [&scene,i]() 
						{
						GD_Renderer3D::DeleteMaterial(i, [&scene](uint32_t defaultMat,EntityID entityID)
							{
								auto& registry = scene->GetRegistry();

								auto view = registry.view<Sol::MaterialComp>();
								for (auto viewEntityID : view)
								{
									if (viewEntityID != entityID) { continue; }
									auto& matComp = view.get<Sol::MaterialComp>(viewEntityID);
									matComp.SwapMaterial(0, entityID);
								}
							});
					} });
				}
			}

			DrawDropDownList("MaterialSelection", "Select Material", buttons, deleteActions);
			ImGui::NextColumn();

			ImGui::Columns(2);
			ImGui::SetColumnWidth(0, 80.f);
			ImGui::Text("Diffuse");
			ImGui::NextColumn();

			if (!readOnly)
			{
				auto& path = component.GetBaseTexturePath();
				std::string displayPath = "											";
				if (path != "") { displayPath = path; }

				if (ImGui::Button(displayPath.c_str()))
				{

					std::string filePath = FileDialogs::OpenFile("png (*.png)\0*.png\0");
					if (!filePath.empty())
					{
						auto cleanPath = CleanUpFilePath(filePath);
						auto matIndex = component.GetMaterialIndex();
						auto props = component.Properties;
						component = MaterialComp(cleanPath, matIndex, entityID);
						component.Properties = props;
					}
				}
			}
			else
			{
				ImGui::TextDisabled("No File Path");
			}
			ImGui::Columns(1);

			auto& color = component.Properties.Color;
			DrawVec4Control("Color", color, 1.f, 0.01f, 0.f, 1.f, { "R","G","B","A" });
			});
	}
	std::string PropertiesPanel::CleanUpFilePath(std::string& filePath)
	{
		std::string cleaned_path; // output path with forward slashes

								  // Find the position of the last backslash in the path
		size_t last_slash = filePath.find_last_of('\\');

		// Extract the substring starting from the character after the last backslash
		std::string filename = filePath.substr(last_slash + 1);

		// Find the position of the "assets\\" substring
		size_t assets_pos = filePath.find("assets\\");

		// Extract the substring starting from the "assets\\" substring
		std::string directory = filePath.substr(assets_pos);

		// Replace backslashes with forward slashes in the directory string
		std::replace(directory.begin(), directory.end(), '\\', '/');

		// Concatenate the cleaned up directory and filename strings
		cleaned_path = directory;
		return cleaned_path;
	}
}
