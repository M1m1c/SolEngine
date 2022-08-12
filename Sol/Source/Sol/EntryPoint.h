#pragma once

#ifdef SOL_PLATFORM_WINDOWS

extern Sol::Application* Sol::CreateApplication();

int main(int argc, char** argv)
{
	auto app = Sol::CreateApplication();
	app->Run();
	delete app;
}

#endif // SOL_PLATFORM_WINDOWS
