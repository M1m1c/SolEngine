#pragma once

#ifdef NEB_PLATFORM_WINDOWS

extern Nebula::Application* Nebula::CreateApplication();

int main(int argc, char** argv)
{
	auto app = Nebula::CreateApplication();
	app->Run();
	delete app;
}

#endif // NEB_PLATFORM_WINDOWS
