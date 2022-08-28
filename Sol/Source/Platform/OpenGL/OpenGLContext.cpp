#include "solpch.h"
#include "OpenGLContext.h"
#include "Sol/Core.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

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
	}

	void OpenGLContext::SwapBuffers()
	{

		glfwSwapBuffers(m_WindowHandle);
	}

}