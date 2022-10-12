#include "Sol.h"
#include "Sol/Core/EntryPoint.h"

#include "EditorLayer.h"
namespace Sol 
{
	class SolEditorApp : public Application
	{
	public:
		SolEditorApp() : 
			Application("Sol-Editor")
		{
			PushLayer(new EditorLayer());
		}

		~SolEditorApp() {}
	};

	Application* CreateApplication()
	{
		return new SolEditorApp();
	}
}

