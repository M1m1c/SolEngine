#pragma once

#include "Sol.h"

#include "Panels/SceneHierarchyPanel.h"

namespace Sol
{
	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		virtual void OnUpdate(TimeStep deltaTime) override;
		virtual void OnFixedUpdate(TimeStep fixedStep, const float fixedTime) override;

		virtual void OnImGuiRender() override;
		
		virtual void OnEvent(Event& e) override;
	private:

		bool OnKeyPressed(KeyPressedEvent& e);

		void CreateNewScene();

		void CreateEditorCamera(s_ptr<Scene> activeScene);

		void OpenScene();

		void SaveSceneAs();

	private:

		u_ptr<GD_Framebuffer> m_Framebuffer;
		s_ptr<Scene> m_ActiveScene;
		Entity m_EditorCameraEntity;

		u_ptr<CameraController> m_CameraController;


		bool m_ViewPortFocused = false;
		bool m_ViewPortHovered = false;

		glm::vec2 m_ViewportSize = { 0,0 };
		glm::vec2 m_ViewportBounds[2];

		int m_GizmoType = 0;

		//Panels
		SceneHierarchyPanel m_HierarchyPanel;
		PropertiesPanel m_PropertiesPanel;
	};
}