#pragma once
#include "Sol/Core/Core.h"
#include "Sol/Core/Log.h"
#include "Sol/Scene/Entity.h"

namespace Sol 
{
	class PropertiesPanel
	{
	public:
		PropertiesPanel() = default;
		~PropertiesPanel() = default;


		void OnImGuiRender();

		void SetCurrentSelection(Entity selection) { m_CurrentSelection = selection; }

	private:
		void DrawComponentNodes(Entity entity);

		std::string CleanUpFilePath(std::string& filePath);

	private:
		Entity m_CurrentSelection;
	};
}