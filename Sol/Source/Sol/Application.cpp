
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
	}

	Application::~Application()
	{
	}
	void Application::Run()
	{
		while (m_Running)
		{
			glClear(GL_COLOR_BUFFER_BIT);

			for (Layer* layer : m_LayerStack)
			{
				layer->OnUpdate();
			}

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
