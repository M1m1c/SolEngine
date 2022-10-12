
#include "solpch.h"
#include "Application.h"
#include "Sol/Core/Input.h"
#include "GalaxyDraw/GalaxyDraw.h"
#include <GLFW/glfw3.h>

namespace Sol
{

	Application* Application::s_Instance = nullptr;



	Application::Application(const std::string& name)
	{
		SOL_CORE_ASSERT(!s_Instance, "Application already exists!")
			s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create(WindowProps(name)));
		m_Window->SetEventCallback(SOL_BIND_EVENT_FN(Application::OnEvent));

		GD_Renderer::Init();

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
	}

	Application::~Application()
	{
	}

	//TODO implement FixedUpdate
	void Application::Run()
	{
		while (m_Running)
		{
			if (!m_Minimized)
			{
				float currentTime = glfwGetTime();//Should be in a platform class
				TimeStep deltaTime = currentTime - m_LastFrameTime;
				m_LastFrameTime = currentTime;

				TimeStep frameTime = deltaTime;
				if (frameTime > 0.25) { frameTime = 0.25; }

				m_AccumulatedTime += frameTime;

				while (m_AccumulatedTime >= m_FixedUpdateTime)
				{
					//TODO need to introduce the concept of states on objects, 
					// that holds things such as positions and other physics related things.
					// These states will be used for interpolating between just before OnUpdate.
					//previousState = currentState;

					for (Layer* layer : m_LayerStack)
					{
						layer->OnFixedUpdate(m_FixedTimeStep, m_FixedUpdateTime);
					}
					m_FixedTimeStep += m_FixedUpdateTime;
					m_AccumulatedTime -= m_FixedUpdateTime;
				}

				/*	const float alpha = m_AccumulatedTime / m_FixedUpdateTime;

					State state = currentState * alpha +
						previousState * (1.0 - alpha);

					OnUpdateRender(state);*/

				for (Layer* layer : m_LayerStack)
				{
					layer->OnUpdate(deltaTime);
				}
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
		dispatcher.Dispatch<WindowResizeEvent>(SOL_BIND_EVENT_FN(Application::OnWindowResize));

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



	void Application::Close()
	{
		m_Running = false;
	}

	bool Application::OnWindowClose(WindowClosedEvent& e)
	{
		m_Running = false;
		return true;
	}
	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		if (e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			m_Minimized = true;
			return false;
		}
		m_Minimized = false;
		GD_Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());

		return false;
	}
}
