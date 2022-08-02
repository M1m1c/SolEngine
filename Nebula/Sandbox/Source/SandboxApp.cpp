#include "Nebula.h"

class Sandbox : public Nebula::Application
{
public:
	Sandbox(){}
	~Sandbox(){}
};

Nebula::Application* Nebula::CreateApplication()
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