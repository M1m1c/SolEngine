#include "solpch.h"
#include "OpenGLContext.h"
#include "Sol/Core.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <Sol/Log.h>

namespace Sol 
{
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle) : m_WindowHandle(windowHandle)
	{
		SOL_CORE_ASSERT(windowHandle, "Window Handle is null");
	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		SOL_CORE_ASSERT(status, "Failed to initialize Glad!");

		SOL_CORE_TRACE("OpenGL Info:");
		SOL_CORE_TRACE(" -Vendor: {0}", glGetString(GL_VENDOR));
		SOL_CORE_TRACE(" -Renderer: {0}", glGetString(GL_RENDERER));
		SOL_CORE_TRACE(" -Version: {0}", glGetString(GL_VERSION));


	}

	void OpenGLContext::SwapBuffers()
	{

		glfwSwapBuffers(m_WindowHandle);
	}

}