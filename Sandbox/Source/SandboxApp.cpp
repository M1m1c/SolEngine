#include "Sol.h"

class Sandbox : public Sol::Application
{
public:
	Sandbox(){}
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