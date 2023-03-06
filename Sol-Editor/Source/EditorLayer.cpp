#include "EditorLayer.h"

#include <imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
namespace Sol
{
	EditorLayer::EditorLayer() :
		Layer("Example")
	{
		

	}

	void EditorLayer::OnAttach()
	{
		GD_FramebufferProps properties;
		properties.Width = 1280;
		properties.Height = 720;
		m_Framebuffer = GD_Framebuffer::Create(properties);

		m_ActiveScene = std::make_shared<Scene>();

		
		for (size_t i = 0; i < 3; i++)
		{

			std::string name = "Cube" + std::to_string(i);
			auto TestEntity = m_ActiveScene->CreateEntity(name);
			TestEntity.AddComponent<ModelComp>("assets/models/cube.fbx", TestEntity.GetID());
			TestEntity.AddComponent<MaterialComp>();
			auto& entityTransform = TestEntity.GetComponent<TransformComp>();
			auto& entityMaterial = TestEntity.GetComponent<MaterialComp>();

			if (i == 0) {
				entityTransform.Position = glm::vec3(2.f, 2.f, 2.f);
				entityTransform.Rotation = glm::vec3(-45.f, 45.f, 45.f);
				entityTransform.Scale = glm::vec3(0.5f, 0.5f, 0.5f);
				entityMaterial.Color = glm::vec4(1.f, 0.f, 0.f, 1.f);
			}
			else if (i==1)
			{
				entityTransform.Position = glm::vec3(0.f, 0.f, 0.f);
				entityTransform.Scale = glm::vec3(0.5f, 0.5f, 0.5f);
				entityTransform.Rotation = glm::vec3(-90.f, 0.f, 0.f);
				entityMaterial.Color = glm::vec4(0.f, 0.f, 1.f, 1.f);
			}
			else if (i == 2)
			{
				entityTransform.Position = glm::vec3(-2.f, -2.f, -2.f);
				entityTransform.Scale = glm::vec3(0.5f, 0.5f, 0.5f);
				entityTransform.Rotation = glm::vec3(-90.f, -45.f, -45.f);
				entityMaterial.Color = glm::vec4(0.f, 1.f, 0.f, 1.f);
			}
			
		}


		m_CameraEntity = m_ActiveScene->CreateEntity("Camera Entity");
		auto& camTransform = m_CameraEntity.GetComponent<TransformComp>();
		auto& sceneCam = m_CameraEntity.AddComponent<CameraComp>();
		auto& intComp = m_CameraEntity.AddComponent<InternalComp>();

		camTransform.Position = glm::vec3(0.f, 0.f, -5.f);
		m_CameraController = std::make_unique<CameraController>(camTransform, sceneCam);

		m_HierarchyPanel.SetCurrentScene(m_ActiveScene);
	}

	void EditorLayer::OnDetach()
	{
	}

	void EditorLayer::OnUpdate(TimeStep deltaTime)
	{
		SOL_PROFILE_FUNCTION();
		

		//RESIZE
		if(m_ViewPortSize.x>0.0f && m_ViewPortSize.y>0.0f) 
		{
			m_ActiveScene->OnViewportResize((uint32_t)m_ViewPortSize.x, (uint32_t)m_ViewPortSize.y);


			GD_FramebufferProps props = m_Framebuffer->GetProperties();
			bool viewLargerThanZero = m_ViewPortSize.x > 0.0f && m_ViewPortSize.y > 0.0f;
			bool propsDontMatch = props.Width != m_ViewPortSize.x || props.Height != m_ViewPortSize.y;

			if (viewLargerThanZero && propsDontMatch) 
			{
				m_Framebuffer->Resize((uint32_t)m_ViewPortSize.x, (uint32_t)m_ViewPortSize.y);
			}
		}

		//TODO tie camera controller to new entity with camera comp
		if (m_ViewPortFocused)
		{
			m_CameraController->OnUpdate(deltaTime);
		}

		m_Framebuffer->Bind();

		//RENDER STEP
		GD_RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		GD_RenderCommand::Clear();

		//GD_Renderer2D::BeginScene(m_CameraController.GetCamera());

		m_ActiveScene->OnUpdate(deltaTime);
	

		//GD_Renderer2D::EndScene();

		m_Framebuffer->UnBind();
	}

	void EditorLayer::OnFixedUpdate(TimeStep fixedStep, const float fixedTime)
	{
	}

	void EditorLayer::OnImGuiRender()
	{

		static bool dockSpaceOpen = true;
		static bool opt_fullscreen = true;
		static bool opt_padding = false;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			const ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->WorkPos);
			ImGui::SetNextWindowSize(viewport->WorkSize);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}
		else
		{
			dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
		}

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
		// and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		if (!opt_padding)
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &dockSpaceOpen, window_flags);
		if (!opt_padding)
			ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// Submit the DockSpace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Exit")) { Sol::Application::Get().Close(); }
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		//All windows and tabs need to be here___________________________________________

		m_HierarchyPanel.OnImGuiRender();

		{
			ImGui::Begin("Test");
			ImGui::Text("test stats");
			ImGui::End();
		}

		{
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0,0 });
			ImGui::Begin("ViewPort");

			m_ViewPortFocused = ImGui::IsWindowFocused();
			m_ViewPortHovered = ImGui::IsWindowHovered();
			Application::Get().GetImGuiLayer()->SetBlockEvents(!m_ViewPortFocused || !m_ViewPortHovered);

			ImVec2 size = ImGui::GetContentRegionAvail();
			if (m_ViewPortSize != *((glm::vec2*)&size))
			{
				m_Framebuffer->Resize((uint32_t)size.x, (uint32_t)size.y);
				m_ViewPortSize = { size.x,size.y };
			}

			uint32_t textureID = m_Framebuffer->GetColorAttachmentsRendererID();
			ImGui::Image((void*)textureID, ImVec2{ m_ViewPortSize.x, m_ViewPortSize.y }, ImVec2{ 0,1 }, ImVec2{ 1,0 });

			ImGui::End();
			ImGui::PopStyleVar();
		}

		//All windows and tabs need to be here___________________________________________
		ImGui::End();
	}

	void EditorLayer::OnEvent(Event& e)
	{
		//SOL_TRACE("{0}", event);
		m_CameraController->OnEvent(e);
	}

}