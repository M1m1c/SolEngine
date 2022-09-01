
#include "solpch.h"
#include "Application.h"
#include "Sol/Log.h"
#include <glad/glad.h>

#include "Sol/Input.h"

namespace Sol
{

	Application* Application::s_Instance = nullptr;

	

	Application::Application()
	{
		SOL_CORE_ASSERT(!s_Instance, "Application already exists!")
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(SOL_BIND_EVENT_FN(Application::OnEvent));

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);

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

		/*std::string vertexSrc = R"(
		#version 330 core

		layout(location = 0) in vec3 a_Position;
		out vec3 v_Position;
		void main()
		{
			v_Position = a_Position + 0.5;
			gl_Position = vec4(a_Position, 1.0);
		}
			
		)";*/


	/*	std::string fragmentSrc = R"(
		#version 330 core

		layout(location = 0) out vec4 color;
		in vec3 v_Position;
		
		void main()
		{
			color = vec4(v_Position, 1.0);
		}
			
		)";*/

		m_Shader.reset(new Shader(
			"../Triangle.vert",
			"../Triangle.frag"));
	}

	Application::~Application()
	{
	}
	void Application::Run()
	{
		while (m_Running)
		{
			glClear(GL_COLOR_BUFFER_BIT);

			m_Shader->Bind();
			m_VertexArray->Bind();

			glDrawElements(GL_TRIANGLES, m_VertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);

			for (Layer* layer : m_LayerStack)
			{
				layer->OnUpdate();
			}

			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
			{
				layer->OnImGuiRender();
			}
			m_ImGuiLayer->End();

			/*auto [x, y] = Input::GetMousePosition();
			SOL_CORE_TRACE("{0}, {1}", x, y);*/

			m_Window->OnUpdate();
		}
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);

		dispatcher.Dispatch<WindowClosedEvent>(SOL_BIND_EVENT_FN(Application::OnWindowClose));

		/*SOL_CORE_TRACE("{0}", e);*/

		for (auto i = m_LayerStack.end(); i != m_LayerStack.begin();)
		{
			(*--i)->OnEvent(e);
			if (e.Handled) { break; }
		}
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* overlay)
	{
		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}

	

	bool Application::OnWindowClose(WindowClosedEvent& e)
	{
		m_Running = false;
		return true;
	}
}
