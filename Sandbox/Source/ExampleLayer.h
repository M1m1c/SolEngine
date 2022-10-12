#pragma once
#include "Sol.h"


class ExampleLayer : public Sol::Layer
{
public:
	ExampleLayer();

	virtual void OnUpdate(Sol::TimeStep deltaTime) override;
	
	virtual void OnFixedUpdate(Sol::TimeStep fixedStep, const float fixedTime) override;
	
	//TODO move into editor project
	virtual void OnImGuiRender() override;
	

	virtual void OnEvent(Sol::Event& e) override;
	

	virtual void OnAttach() override;
	

private:

	GD_ShaderLibrary m_ShaderLib;
	Sol::s_ptr<GD_Shader> m_Shader;
	Sol::s_ptr<GD_VAO> m_VertexArray;
	Sol::s_ptr<GD_Texture2D> m_Texture;
	Sol::s_ptr<GD_Texture2D> m_WhiteTexture;

	Sol::u_ptr<GD_Framebuffer> m_Framebuffer;

	glm::vec3 m_TrianglePos = glm::vec3(0.f);

	Sol::CameraController m_CameraController;
	//glm::vec3 m_CameraPosition = glm::vec3(0.f);
	//float m_CameraSpeed = 1.f;

	glm::vec3 m_TriangleColor = { 0.f, 0.8f, 0.8f };
};