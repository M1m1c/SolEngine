#pragma once
#include "Log.h"

#ifdef SOL_PLATFORM_WINDOWS

extern Sol::Application* Sol::CreateApplication();

int main(int argc, char** argv)
{
	Sol::Log::Init();
	SOL_CORE_WARN("initalised log!");
	int a = 5;
	SOL_WARN("Hello! Var={0}",a);

	auto app = Sol::CreateApplication();
	app->Run();
	delete app;
}

#endif // SOL_PLATFORM_WINDOWS
