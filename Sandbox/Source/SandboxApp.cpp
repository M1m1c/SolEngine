#include "Sol.h"

#include <imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class ExampleLayer : public Sol::Layer
{
public:
	ExampleLayer() : Layer("Example"), m_Camera(10, 10, glm::vec3(0.f))
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

		uint32_t indices[3] = { 0,1,2 };
		auto indexBuffer = GD_EBO::Create(indices, sizeof(indices) / sizeof(uint32_t));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		m_Shader = GD_Shader::Create(
			"Square.vert",
			"Square.frag");

		m_Texture = GD_Texture2D::Create("assets/textures/think.png");
		
		m_Shader->Bind();
		m_Shader->setInt("u_Texture", 0);
		
	}

	void OnFixedUpdate(Sol::TimeStep fixedStep, const float fixedTime) override
	{

	}

	void OnUpdate(Sol::TimeStep deltaTime) override
	{

		if (Sol::Input::IsKeyPressed(SOL_KEY_D))
		{
			m_CameraPosition.x += m_CameraSpeed * deltaTime;
		}

		if (Sol::Input::IsKeyPressed(SOL_KEY_A))
		{
			m_CameraPosition.x += -m_CameraSpeed * deltaTime;
		}

		if (Sol::Input::IsKeyPressed(SOL_KEY_W))
		{
			m_CameraPosition.y += m_CameraSpeed * deltaTime;
		}

		if (Sol::Input::IsKeyPressed(SOL_KEY_S))
		{
			m_CameraPosition.y += -m_CameraSpeed * deltaTime;
		}

		//SOL_INFO("ExampleLayer::Update");
		GD_RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		GD_RenderCommand::Clear();

		m_Camera.SetPosition(m_CameraPosition);

		m_Camera.SetRotation({ 0.5f,0.0f,0.f });

		static glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		GD_Renderer::BeginScene(m_Camera);

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
		GD_Renderer::Submit(m_Shader, m_VertexArray, transform);

		m_Texture->Bind();
		GD_Renderer::EndScene();
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Triangle Color", glm::value_ptr(m_TriangleColor));
		ImGui::End();
	}

	virtual void OnEvent(Sol::Event& event) override
	{
		//SOL_TRACE("{0}", event);
	}

private:
	Sol::s_ptr<GD_Shader> m_Shader;
	Sol::s_ptr<GD_VAO> m_VertexArray;
	Sol::s_ptr<GD_Texture2D> m_Texture;

	glm::vec3 m_TrianglePos = glm::vec3(0.f);

	GD_Camera m_Camera;
	glm::vec3 m_CameraPosition = glm::vec3(0.f);
	float m_CameraSpeed = 1.f;

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