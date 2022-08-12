#pragma once

#include "Core.h";

namespace Sol 
{
	class SOL_API Application
	{
	public:
		Application();
		virtual ~Application();
		void Run();
	};

	Application* CreateApplication();
}
