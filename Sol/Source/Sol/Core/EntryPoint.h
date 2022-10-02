#pragma once
#include "Log.h"

#ifdef SOL_PLATFORM_WINDOWS

extern Sol::Application* Sol::CreateApplication();

int main(int argc, char** argv)
{
	Sol::Log::Init();

	SOL_PROFILE_BEGIN_SESSION("Startup", "SolProfile-Startup.json");
	auto app = Sol::CreateApplication();
	SOL_PROFILE_END_SESSION();

	SOL_PROFILE_BEGIN_SESSION("Runtime", "SolProfile-Runtime.json");
	app->Run();
	SOL_PROFILE_END_SESSION();

	SOL_PROFILE_BEGIN_SESSION("Shutdown", "SolProfile-Shutdown.json");
	delete app;
	SOL_PROFILE_END_SESSION();
}

#endif // SOL_PLATFORM_WINDOWS
