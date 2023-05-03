#include "EditorLayer.h"

#include "Sol/Scene/SceneSerializer.h"
#include "Sol/Utils/PlatformUtils.h"
#include "GalaxyDraw/Camera.h"
#include "Sol/Math/Math.h"

#include <imgui.h>
#include <ImGuizmo.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Sol
{
	EditorLayer::EditorLayer() : Layer("Example") {	}

	void EditorLayer::OnAttach()
	{
		GD_FramebufferProps properties;
		properties.Attachments = { GD_FramebufferTextureFormat::RGBA8,GD_FramebufferTextureFormat::RED_INTERGER,GD_FramebufferTextureFormat::Depth };
		properties.Width = 1280;
		properties.Height = 720;
		m_Framebuffer = GD_Framebuffer::Create(properties);

		m_HierarchyPanel.SetPropertiesPanel(&m_PropertiesPanel);
		m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;

		CreateNewScene();

		//SceneSerializer serializer(m_ActiveScene);
		//serializer.SerializeToText("assets/scenes/Example.scene");
		//serializer.DeserializeText("assets/scenes/Example.scene");
	}

	void EditorLayer::OnDetach()
	{
	}

	void EditorLayer::OnUpdate(TimeStep deltaTime)
	{
		SOL_PROFILE_FUNCTION();


		//RESIZE
		if (m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f)
		{
			m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_CameraController->SetCenterOfView(m_ViewportSize);

			GD_FramebufferProps props = m_Framebuffer->GetProperties();
			bool viewLargerThanZero = m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f;
			bool propsDontMatch = props.Width != m_ViewportSize.x || props.Height != m_ViewportSize.y;

			if (viewLargerThanZero && propsDontMatch)
			{
				m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);

			}
		}

		if (m_ViewPortFocused)
		{
			m_CameraController->OnUpdate(deltaTime);
		}

		m_Framebuffer->Bind();

		//RENDER STEP
		GD_RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		GD_RenderCommand::Clear();

		//Clear EntityID attachment to -1
		m_Framebuffer->ClearAttachment(1, -1);

		//UPDATE SCENE
		m_ActiveScene->OnUpdateRuntime(deltaTime);

		CheckMouseHover();

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

		ImGuiStyle& style = ImGui::GetStyle();
		style.WindowMinSize.x = 370.f;

		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		style.WindowMinSize.x = 32.f;

		if (ImGui::BeginMenuBar())
		{

			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("New...", "Ctrl+N")) { CreateNewScene(); }

				if (ImGui::MenuItem("Open...", "Ctrl+O")) { OpenScene(); }

				if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S")) { SaveSceneAs(); }

				if (ImGui::MenuItem("Exit")) { Sol::Application::Get().Close(); }
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		//All windows and tabs need to be here___________________________________________

		m_HierarchyPanel.OnImGuiRender();
		m_PropertiesPanel.OnImGuiRender();

		//{
		//	/*ImGui::Begin("Test");
		//	ImGui::Text("test stats");
		//	ImGui::End();*/
		//}


		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0,0 });
		ImGui::Begin("ViewPort");

		auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
		auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
		auto viewportOffset = ImGui::GetWindowPos();//Includes tab bar
		m_ViewportBounds[0] = { viewportMinRegion.x + viewportOffset.x,viewportMinRegion.y + viewportOffset.y };
		m_ViewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x,viewportMaxRegion.y + viewportOffset.y };

		m_ViewPortFocused = ImGui::IsWindowFocused();
		m_ViewPortHovered = ImGui::IsWindowHovered();
		Application::Get().GetImGuiLayer()->SetBlockEvents(!m_ViewPortFocused && !m_ViewPortHovered);

		ImVec2 size = ImGui::GetContentRegionAvail();
		if (m_ViewportSize != *((glm::vec2*)&size))
		{
			m_Framebuffer->Resize((uint32_t)size.x, (uint32_t)size.y);
			m_ViewportSize = { size.x,size.y };
		}

		uint32_t textureID = m_Framebuffer->GetColorAttachmentsRendererID();
		ImGui::Image((void*)textureID, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0,1 }, ImVec2{ 1,0 });




		//GIZMOS__________________________________
		Entity selectedEntity = m_HierarchyPanel.GetCurrentSelectedEntity();
		if (selectedEntity && m_GizmoType != -1)
		{
			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();

			ImGuizmo::SetRect(m_ViewportBounds[0].x, m_ViewportBounds[0].y, m_ViewportBounds[1].x - m_ViewportBounds[0].x, m_ViewportBounds[1].y - m_ViewportBounds[0].y);

			const auto& cameraTransform = m_EditorCameraEntity.GetComponent<TransformComp>();
			auto& camera = m_EditorCameraEntity.GetComponent<CameraComp>().Camera;
			auto camView = cameraTransform.GetViewMatrix();
			auto camProj = camera.GetProjection();


			auto& selectedTransform = selectedEntity.GetComponent<TransformComp>();
			auto transformMatrix = selectedTransform.GetTransformMatrix();

			bool isSnapping = Input::IsKeyPressed(Key::LEFT_CONTROL);
			float snapValue = m_GizmoType == ImGuizmo::OPERATION::ROTATE ? 5.f : 0.5f;
			float snapValues[3] = { snapValue,snapValue,snapValue };

			ImGuizmo::Manipulate(
				glm::value_ptr(camView),
				glm::value_ptr(camProj),
				(ImGuizmo::OPERATION)m_GizmoType,
				ImGuizmo::LOCAL,
				glm::value_ptr(transformMatrix),
				nullptr,
				isSnapping ? snapValues : nullptr);

			if (ImGuizmo::IsUsing())
			{
				glm::vec3 position, rotation, scale;
				Math::DecomposeTransform(transformMatrix, position, rotation, scale);

				selectedTransform.Position = position;
				selectedTransform.Scale = scale;

				//TODO ROtation needs additional fixing here, sometimes display numbers go haywire when rotating
				auto deltaRot = selectedTransform.Rotation - glm::degrees(rotation);
				selectedTransform.Rotation -= deltaRot;
			}
		}

		ImGui::End();
		ImGui::PopStyleVar();

		ImGui::End();
	}

	void EditorLayer::OnEvent(Event& e)
	{
		//SOL_TRACE("{0}", event);
		m_CameraController->OnEvent(e);

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<KeyPressedEvent>(SOL_BIND_EVENT_FN(EditorLayer::OnKeyPressed));
		dispatcher.Dispatch<MouseButtonPressedEvent>(SOL_BIND_EVENT_FN(EditorLayer::OnMouseButtonPressed));
	}

	bool EditorLayer::OnKeyPressed(KeyPressedEvent& e)
	{
		//SHORTCUTS
		if (e.GetRepeatCount() > 0)
		{
			return false;
		}

		bool control = Input::IsKeyPressed(Key::LEFT_CONTROL) || Input::IsKeyPressed(Key::RIGHT_CONTROL);
		bool shift = Input::IsKeyPressed(Key::LEFT_SHIFT) || Input::IsKeyPressed(Key::RIGHT_SHIFT);

		//TODO make some of these bindable
		switch (e.GetKeyCode())
		{
		case Key::N:
			if (control) { CreateNewScene(); }
			break;

		case Key::O:
			if (control) { OpenScene(); }
			break;

		case Key::S:
			if (control && shift) { SaveSceneAs(); }
			break;

		case Key::G:
			m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
			break;

		case Key::F:
			m_GizmoType = ImGuizmo::OPERATION::SCALE;
			break;

		case Key::R:
			m_GizmoType = ImGuizmo::OPERATION::ROTATE;
			break;
		}
	}

	bool EditorLayer::OnMouseButtonPressed(MouseButtonPressedEvent& e)
	{
		if (e.GetMouseButton() == MouseButton::BUTTON_LEFT)
		{
			if (CanMouseSelectEntity())
				m_HierarchyPanel.SetCurrentSelectedEntity(m_HoveredEntity);

		}

		return false;
	}

	bool EditorLayer::CanMouseSelectEntity()
	{
		return m_ViewPortHovered && m_ViewPortFocused && !ImGuizmo::IsOver();
	}

	void EditorLayer::CheckMouseHover()
	{
		auto [mx, my] = ImGui::GetMousePos();
		mx -= m_ViewportBounds[0].x;
		my -= m_ViewportBounds[0].y;
		glm::vec2 viewportSize = m_ViewportBounds[1] - m_ViewportBounds[0];
		my = viewportSize.y - my;
		int mouseX = (int)mx;
		int mouseY = (int)my;

		bool isMouseInViewportPanel = mouseX >= 0 && mouseY >= 0 && mouseX < (int)viewportSize.x&& mouseY < (int)viewportSize.y;
		if (isMouseInViewportPanel)
		{
			int pixelData = m_Framebuffer->ReadPixel(1, mouseX, mouseY);
			//TODO since we are using signed ints now we won't be able to select every entity in the scene,
			//in the future we might want to change so we render unsigned ints in the framebuffer.
			m_HoveredEntity = pixelData < 0 ? Entity() : Entity((EntityID)pixelData, m_ActiveScene.get());
		}
	}

	void EditorLayer::CreateNewScene()
	{
		if (m_ActiveScene)
		{
			m_ActiveScene->DestroyAllEntities();
		}

		m_ActiveScene = std::make_shared<Scene>();
		m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		m_HierarchyPanel.SetCurrentScene(m_ActiveScene);
		CreateEditorCamera(m_ActiveScene);
		//TODO create a lightdirection entity or make it so we cna set on up through imgui
	}

	void EditorLayer::CreateEditorCamera(s_ptr<Scene> activeScene)
	{
		m_EditorCameraEntity = activeScene->CreateEntity("Editor Camera");
		auto& camTransform = m_EditorCameraEntity.GetComponent<TransformComp>();
		auto& sceneCam = m_EditorCameraEntity.AddComponent<CameraComp>();
		auto& intComp = m_EditorCameraEntity.AddComponent<InternalComp>();

		camTransform.Position = glm::vec3(0.f, 0.f, -5.f);
		m_CameraController = std::make_unique<CameraController>(camTransform, sceneCam);
	}

	void EditorLayer::OpenScene()
	{
		std::string filePath = FileDialogs::OpenFile("Sol Scene (*.scene)\0*.scene\0");
		if (!filePath.empty())
		{
			CreateNewScene();

			SceneSerializer serializer(m_ActiveScene);
			serializer.DeserializeText(filePath);
		}
	}

	void EditorLayer::SaveSceneAs()
	{
		std::string filePath = FileDialogs::SaveFile("Sol Scene (*.scene)\0*.scene\0");
		if (!filePath.empty())
		{
			SceneSerializer serializer(m_ActiveScene);
			serializer.SerializeToText(filePath);
		}
	}
}