#include "Application.h"

#include "Sol/Events/ApplicationEvent.h"
#include "Sol/Log.h"

namespace Sol 
{
	Application::Application()
	{
	}

	Application::~Application()
	{
	}
	void Application::Run()
	{
		WindowResizeEvent temp(1920, 1080);
		SOL_TRACE(temp);

		while (true)
		{

		}
	}
}
