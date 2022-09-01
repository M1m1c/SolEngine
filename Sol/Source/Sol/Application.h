#pragma once

#include "Core.h";
#include "Window.h";
#include "LayerStack.h"
#include "Events/Event.h";
#include "Sol/Events/ApplicationEvent.h"
#include "Sol/ImGui/ImGuiLayer.h"

//TEMPORARY, WILL BE REMOVED LATER
//#include "Renderer/Shader.h"
#include <GalaxyDraw/GalaxyDraw.h>

namespace Sol 
{
	class SOL_API Application
	{
	public:
		Application();
		virtual ~Application();
		void Run();
		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		static inline Application& Get() { return *s_Instance; }
		inline Window& GetWindow() { return *m_Window; }

	private:

		bool OnWindowClose(WindowClosedEvent& e);

		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		LayerStack m_LayerStack;

		std::shared_ptr<Shader> m_Shader;
		std::shared_ptr<GD_VAO> m_VertexArray;
	

		static Application* s_Instance;
	};

	Application* CreateApplication();
}
