#include "Sol.h"

#include <imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class ExampleLayer : public Sol::Layer
{
public:
	ExampleLayer() : Layer("Example"), m_CameraController(10, 10,glm::vec2(1.6f,0.9f), glm::vec3(0.f))
	{
		m_VertexArray = GD_VAO::Create();

		float vertices[3 * 3] = {
			-0.5f, -0.5f, 0.0f,
			0.5f, -0.5f, 0.0f,
			0.0f, 0.5f, 0.0f
		};

		float squareVertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.f, 0.f,
			0.5f, -0.5f, 0.0f, 1.f, 0.f,
			0.5f, 0.5f, 0.0f, 1.f, 1.f,
			-0.5f, 0.5f, 0.0f, 0.f, 1.f,
		};

		auto vertexBuffer = GD_VBO::Create(squareVertices, sizeof(squareVertices));

		GD_BufferLayout layout =
		{
			{GD_ShaderDataType::Float3, "a_Position"},
			{GD_ShaderDataType::Float2, "a_TexCoord"},
			/*	{GD_ShaderDataType::Float3, "a_Normal"},
				{GD_ShaderDataType::Float4, "a_Color"},*/
		};

		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t indices[6] = { 0,1,2,2,3,0};
		auto indexBuffer = GD_EBO::Create(indices, sizeof(indices) / sizeof(uint32_t));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		auto shader = m_ShaderLib.Load("Square", "Square.vert", "Square.frag");

		m_Texture = GD_Texture2D::Create("assets/textures/think.png");
		
		shader->Bind();
		shader->setInt("u_Texture", 0);
		
	}

	void OnFixedUpdate(Sol::TimeStep fixedStep, const float fixedTime) override
	{

	}

	void OnUpdate(Sol::TimeStep deltaTime) override
	{
		//UPDATE STEP
		m_CameraController.OnUpdate(deltaTime);
		

		//RENDER STEP
		GD_RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		GD_RenderCommand::Clear();

		static glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		GD_Renderer::BeginScene(m_CameraController.GetCamera());

		//m_Shader->setVec3("u_Color", m_TriangleColor);

	/*	for (int x = -5; x < 5; x++)
		{
			for (int y = -5; y < 5; y++)
			{
				glm::vec3 pos(x * 0.12f, y * 0.12f, 0.f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;

				GD_Renderer::Submit(m_Shader, m_VertexArray, transform);
			}
		}*/
		glm::vec3 pos(0.f,0.f,0.f);
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos);

		auto shader = m_ShaderLib.Get("Square");

		GD_Renderer::Submit(shader, m_VertexArray, transform);

		m_Texture->Bind();
		GD_Renderer::EndScene();
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Triangle Color", glm::value_ptr(m_TriangleColor));
		ImGui::End();
	}

	virtual void OnEvent(Sol::Event& e) override
	{
		//SOL_TRACE("{0}", event);
		m_CameraController.OnEvent(e);
	}

private:

	GD_ShaderLibrary m_ShaderLib;
	Sol::s_ptr<GD_Shader> m_Shader;
	Sol::s_ptr<GD_VAO> m_VertexArray;
	Sol::s_ptr<GD_Texture2D> m_Texture;

	glm::vec3 m_TrianglePos = glm::vec3(0.f);

	Sol::CameraController m_CameraController;
	//glm::vec3 m_CameraPosition = glm::vec3(0.f);
	//float m_CameraSpeed = 1.f;

	glm::vec3 m_TriangleColor = { 0.f, 0.8f, 0.8f };
};

class Sandbox : public Sol::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
	}

	~Sandbox() {}
};

Sol::Application* Sol::CreateApplication()
{
	return new Sandbox();
}

//int main() 
//{
//	auto sandbox = new Sandbox();
//	sandbox->Run();
//	delete sandbox;
//	return 0;
//}