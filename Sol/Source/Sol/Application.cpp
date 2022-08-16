
#include "solpch.h"
#include "Application.h"
#include "Sol/Events/ApplicationEvent.h"
#include "Sol/Log.h"

namespace Sol
{
	Application::Application()
	{
		m_Window = std::unique_ptr<Window>(Window::Create());
	}

	Application::~Application()
	{
	}
	void Application::Run()
	{
		while (m_Running)
		{
			m_Window->OnUpdate();
		}
	}
}
