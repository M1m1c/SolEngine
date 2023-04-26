#pragma once
#include "Sol/Core/Core.h"
#include "Sol/Core/Log.h"
#include "Sol/Scene/Entity.h"


namespace Sol 
{
	class Scene;
	class PropertiesPanel
	{
	public:
		PropertiesPanel() = default;
		~PropertiesPanel() { m_CurrentScene = nullptr; }


		void OnImGuiRender();

		void SetCurrentSelection(Entity selection) { m_CurrentSelection = selection; }
		void SetCurrentScene(Scene* scene) { m_CurrentScene = scene; }

	private:
		void DrawComponentNodes(Entity entity);

		std::string CleanUpFilePath(std::string& filePath);

	private:
		Entity m_CurrentSelection;
		Scene* m_CurrentScene;
	};
}