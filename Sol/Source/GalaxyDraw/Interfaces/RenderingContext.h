#pragma once


struct GLFWwindow;
namespace GalaxyDraw {
	class RenderingContext
	{
	public:
		static std::shared_ptr<RenderingContext> Create(GLFWwindow* windowHandle);
		virtual void Init() = 0;
		virtual void SwapBuffers() = 0;
	};
}