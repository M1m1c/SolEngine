#include "solpch.h"
#include "WindowsWindow.h"
#include "Sol/Log.h"

namespace Sol
{
	static bool s_GLFWInitalised = false;

	Window* Window::Create(const WindowProps& props) 
	{
		return new WindowsWindow(props);
	}

	WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		Init(props);
	}

	WindowsWindow::~WindowsWindow()
	{
		Shutdown();
	}

	

	void WindowsWindow::Init(const WindowProps& props)
	{
		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;

		SOL_CORE_TRACE("Creating Window {0}, ({1},{2}) ", m_Data.Title, m_Data.Width, m_Data.Height);

		if (!s_GLFWInitalised)
		{
			int sucess = glfwInit();
			SOL_CORE_ASSERT(sucess, "Could not initalise GLFW!");

			s_GLFWInitalised = true;
		}
		
		m_Window = glfwCreateWindow(
			(int)props.Width,
			(int)props.Height,
			m_Data.Title.c_str(),
			nullptr,
			nullptr);

		glfwMakeContextCurrent(m_Window);
		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(true);
	}

	void WindowsWindow::Shutdown()
	{
		glfwDestroyWindow(m_Window);
	}
	void WindowsWindow::OnUpdate()
	{
		glfwPollEvents();
		glfwSwapBuffers(m_Window);
	}

	void Sol::WindowsWindow::SetVSync(bool enabled)
	{
		if (enabled) { glfwSwapInterval(1); }
		else { glfwSwapInterval(0); }

		m_Data.VSync = enabled;
	}

	bool Sol::WindowsWindow::IsVSync() const
	{
		return m_Data.VSync;
	}

}