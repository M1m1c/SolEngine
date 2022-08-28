#pragma once

#include "Core.h";
#include "Window.h";
#include "LayerStack.h"
#include "Events/Event.h";
#include "Sol/Events/ApplicationEvent.h"
#include "Sol/ImGui/ImGuiLayer.h"

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

		unsigned int m_VertexArray, m_VertexBuffer, m_IndexBuffer;

		static Application* s_Instance;
	};

	Application* CreateApplication();
}
