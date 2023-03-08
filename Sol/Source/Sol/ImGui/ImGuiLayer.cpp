#include "solpch.h"
#include "ImGuiLayer.h"

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"


#include "Sol/Core/Application.h"

#include "GLFW/glfw3.h"
#include <glad/glad.h>

namespace Sol
{
	ImGuiLayer::ImGuiLayer() : Layer("ImGuiLayer")
	{
	}

	ImGuiLayer::~ImGuiLayer()
	{
	}

	void ImGuiLayer::OnAttach()
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

		io.FontDefault = io.Fonts->AddFontFromFileTTF("assets/fonts/Saira/Saira-SemiBold.ttf", 24.f);
		//"assets/fonts/Roboto/Roboto-Medium.ttf", 18.f);
		//"assets/fonts/Golos/static/GolosText-Medium.ttf", 18.f);

		ImGui::StyleColorsDark();


		ImGuiStyle& style = ImGui::GetStyle();

		//tweak window rounding and background to look identical to regular ones
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) 
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		Application& app = Application::Get();
		GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());

		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 460");
	}

	void ImGuiLayer::OnDetach()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiLayer::OnUpdate(TimeStep deltaTime)
	{
	}

	void ImGuiLayer::OnFixedUpdate(TimeStep fixedStep, const float fixedTime)
	{
	}

	void ImGuiLayer::OnImGuiRender()
	{
		static bool show = true;
	}

	void ImGuiLayer::OnEvent(Event& e)
	{
		if (m_BlockEvents) 
		{ 
			ImGuiIO& io = ImGui::GetIO();
			e.Handled |= e.IsInCategory(EventCategoryMouse) & io.WantCaptureMouse;
			e.Handled |= e.IsInCategory(EventCategoryKeyboard) & io.WantCaptureKeyboard;
		}
	}

	void ImGuiLayer::Begin()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void ImGuiLayer::End()
	{
		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();

		io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_Current_Context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_Current_Context);
		}
	}
	
}