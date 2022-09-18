#include "solpch.h"
#include "OpenGL_Context.h"
#include "Sol/Core/Core.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <Sol/Core/Log.h>

namespace GalaxyDraw 
{
	OpenGL_Context::OpenGL_Context(GLFWwindow* windowHandle) : m_WindowHandle(windowHandle)
	{
		SOL_CORE_ASSERT(windowHandle, "Window Handle is null");
	}

	void OpenGL_Context::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		SOL_CORE_ASSERT(status, "Failed to initialize Glad!");

		SOL_CORE_TRACE("OpenGL Info:");
		SOL_CORE_TRACE(" -Vendor: {0}", glGetString(GL_VENDOR));
		SOL_CORE_TRACE(" -Renderer: {0}", glGetString(GL_RENDERER));
		SOL_CORE_TRACE(" -Version: {0}", glGetString(GL_VERSION));


	}

	void OpenGL_Context::SwapBuffers()
	{

		glfwSwapBuffers(m_WindowHandle);
	}

}