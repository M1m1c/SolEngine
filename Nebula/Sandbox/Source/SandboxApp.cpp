#include "Nebula.h"

class Sandbox : public Nebula::Application
{
public:
	Sandbox(){}
	~Sandbox(){}
};

int main() 
{
	auto sandbox = new Sandbox();
	sandbox->Run();
	delete sandbox;
	return 0;
}