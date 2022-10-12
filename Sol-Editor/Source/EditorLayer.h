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
		Sol::s_ptr<GD_Shader> m_Shader;
		Sol::s_ptr<GD_VAO> m_VertexArray;
		Sol::s_ptr<GD_Texture2D> m_Texture;
		Sol::s_ptr<GD_Texture2D> m_WhiteTexture;

		Sol::u_ptr<GD_Framebuffer> m_Framebuffer;
		Sol::CameraController m_CameraController;
	};
}