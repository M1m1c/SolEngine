#include "PropertiesPanel.h"

#include "Sol/Scene/Components.h"

#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

namespace Sol {

	void PropertiesPanel::OnImGuiRender()
	{
		ImGui::Begin("Properties");
		if (m_CurrentSelection)
		{
			DrawComponentNodes(m_CurrentSelection);
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

			if (ImGui::InputText("Name", buffer, sizeof(buffer))) {
				name = std::string(buffer);
			}
		}

		if (entity.HasComponent<TransformComp>())
		{
			if (ImGui::TreeNodeEx((void*)typeid(TransformComp).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Transform"))
			{
				auto& transform = entity.GetComponent<TransformComp>();

				auto& position = transform.Position;
				ImGui::DragFloat3("Position", glm::value_ptr(position), 0.5f);

				auto& rotation = transform.Rotation;
				ImGui::DragFloat3("Rotation", glm::value_ptr(rotation), 0.5f);

				auto& scale = transform.Scale;
				ImGui::DragFloat3("Scale", glm::value_ptr(scale), 0.5f);

				ImGui::TreePop();
			}	
		}
	}
}
