#pragma once
#include "GalaxyDraw/Interfaces/RenderingContext.h"

struct GLFWwindow;

namespace GalaxyDraw
{
	class OpenGL_Context : public RenderingContext
	{
	public:
		OpenGL_Context(GLFWwindow* windowHandle);

		virtual void Init() override;
		virtual void SwapBuffers() override;
	private:
		GLFWwindow* m_WindowHandle;
	};
}
