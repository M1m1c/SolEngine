#pragma once

#include "Core.h";
#include "Window.h";
#include "LayerStack.h"
#include "Sol/Events/Event.h";
#include "Sol/Events/ApplicationEvent.h"
#include "Sol/ImGui/ImGuiLayer.h"
#include "Sol/Core/TimeStep.h"

//TEMPORARY, WILL BE REMOVED LATER
//#include "Renderer/Shader.h"


namespace Sol 
{
	class SOL_API Application
	{
	public:
		Application(const std::string& name = "Sol App");
		virtual ~Application();
		void Run();
		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		static inline Application& Get() { return *s_Instance; }
		inline Window& GetWindow() { return *m_Window; }

		void Close();

		ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }

	private:

		bool OnWindowClose(WindowClosedEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		bool m_Minimized = false;
		LayerStack m_LayerStack;
		float m_LastFrameTime = 0.0f;

		float m_AccumulatedTime = 0.0f;
		TimeStep m_FixedTimeStep = 0.0f;
		const float m_FixedUpdateTime = 0.01f;

		/*State m_Previous;
		State m_Current;*/

		static Application* s_Instance;
	};

	Application* CreateApplication();
}
