#include "Sol.h"


class ExampleLayer : public Sol::Layer
{
public:
	ExampleLayer() : Layer("Example") {}

	void OnUpdate() override
	{
		SOL_INFO("ExampleLayer::Update");
	}

	virtual void OnEvent(Sol::Event& event) override
	{
		SOL_TRACE("{0}", event);
	}

private:

};

class Sandbox : public Sol::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
		PushOverlay(new Sol::ImGuiLayer());
	}

	~Sandbox(){}
};

Sol::Application* Sol::CreateApplication()
{
	return new Sandbox();
}

//int main() 
//{
//	auto sandbox = new Sandbox();
//	sandbox->Run();
//	delete sandbox;
//	return 0;
//}