#pragma once

#include "Sol.h"

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
		GD_ShaderLibrary m_ShaderLib;
		s_ptr<GD_Shader> m_Shader;
		s_ptr<GD_VAO> m_VertexArray;
		s_ptr<GD_Texture2D> m_Texture;
		s_ptr<GD_Texture2D> m_WhiteTexture;

		u_ptr<GD_Framebuffer> m_Framebuffer;
		u_ptr<Scene> m_ActiveScene;
		Entity m_TempEntity;
		Entity m_CameraEntity;

		CameraController m_CameraController;


		bool m_ViewPortFocused = false;
		bool m_ViewPortHovered = false;

		glm::vec2 m_ViewPortSize = { 0,0 };
	};
}