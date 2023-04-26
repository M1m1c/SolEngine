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

		~SolEditorApp() 
		{
			SOL_CORE_WARN("Application Editor close");
		}
	};

	Application* CreateApplication()
	{
		return new SolEditorApp();
	}
}

