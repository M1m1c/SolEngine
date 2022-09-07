#include "Sol.h"

class ExampleLayer : public Sol::Layer
{
public:
	ExampleLayer() : Layer("Example"), m_Camera(10, 10, glm::vec3(0.f))
	{
		m_VertexArray.reset(GD_VAO::Create());

		float vertices[3 * 3] = {
			-0.5f, -0.5f, 0.0f,
			0.5f, -0.5f, 0.0f,
			0.0f, 0.5f, 0.0f
		};

		std::shared_ptr<GD_VBO> vertexBuffer;
		vertexBuffer.reset(GD_VBO::Create(vertices, sizeof(vertices)));

		GD_BufferLayout layout =
		{
			{GD_ShaderDataType::Float3, "a_Position"},
			/*	{GD_ShaderDataType::Float3, "a_Normal"},
				{GD_ShaderDataType::Float4, "a_Color"},*/
		};

		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t indices[3] = { 0,1,2 };
		std::shared_ptr<GD_EBO> indexBuffer;
		indexBuffer.reset(GD_EBO::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		m_Shader.reset(new GD_Shader(
			"Triangle.vert",
			"Triangle.frag"));
	}

	void OnUpdate() override
	{

		if (Sol::Input::IsKeyPressed(SOL_KEY_D))
		{
			m_CameraPosition.x += m_CameraSpeed;
		}

		if (Sol::Input::IsKeyPressed(SOL_KEY_A))
		{
			m_CameraPosition.x += -m_CameraSpeed;
		}

		if (Sol::Input::IsKeyPressed(SOL_KEY_W))
		{
			m_CameraPosition.y += m_CameraSpeed;
		}

		if (Sol::Input::IsKeyPressed(SOL_KEY_S))
		{
			m_CameraPosition.y += -m_CameraSpeed;
		}


		//SOL_INFO("ExampleLayer::Update");
		GD_RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		GD_RenderCommand::Clear();

		m_Camera.SetPosition(m_CameraPosition);

		m_Camera.SetRotation({ 0.5f,0.0f,0.f });

		GD_Renderer::BeginScene(m_Camera);

		GD_Renderer::Submit(m_Shader, m_VertexArray);

		GD_Renderer::EndScene();
	}

	virtual void OnEvent(Sol::Event& event) override
	{
		//SOL_TRACE("{0}", event);
	}

private:
	std::shared_ptr<GD_Shader> m_Shader;
	std::shared_ptr<GD_VAO> m_VertexArray;

	GD_Camera m_Camera;
	glm::vec3 m_CameraPosition = glm::vec3(0.f);
	float m_CameraSpeed = 0.05f;
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