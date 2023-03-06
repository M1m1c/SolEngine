#include "PropertiesPanel.h"

#include "Sol/Scene/Components.h"
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
	}
}
