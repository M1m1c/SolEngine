#pragma once
#include "Sol.h"


class ExampleLayer : public Sol::Layer
{
public:
	ExampleLayer();


	virtual void OnAttach() override;

	virtual void OnDetach() override;

	virtual void OnUpdate(Sol::TimeStep deltaTime) override;
	
	virtual void OnFixedUpdate(Sol::TimeStep fixedStep, const float fixedTime) override;
	
	virtual void OnImGuiRender() override;
	
	virtual void OnEvent(Sol::Event& e) override;
	

private:

	GD_ShaderLibrary m_ShaderLib;
	Sol::s_ptr<GD_Shader> m_Shader;
	Sol::s_ptr<GD_VAO> m_VertexArray;
	Sol::s_ptr<GD_Texture2D> m_Texture;
	Sol::s_ptr<GD_Texture2D> m_WhiteTexture;

	Sol::CameraController m_CameraController;

	glm::vec3 m_TrianglePos = glm::vec3(0.f);
	glm::vec3 m_TriangleColor = { 0.f, 0.8f, 0.8f };
};