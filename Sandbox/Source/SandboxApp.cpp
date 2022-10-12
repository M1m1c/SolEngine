#include "Sol.h"
#include "Sol/Core/EntryPoint.h"

#include "ExampleLayer.h"

class Sandbox : public Sol::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
	}

	~Sandbox() {}
};

Sol::Application* Sol::CreateApplication()
{
	return new Sandbox();
}
