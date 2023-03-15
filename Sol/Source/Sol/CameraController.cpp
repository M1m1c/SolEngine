#include "solpch.h"
#include "CameraController.h"
#include "Sol/Core/Input.h"
#include "Sol/Core/KeyCodes.h"

namespace Sol
{

	CameraController::CameraController(TransformComp& camTransform, CameraComp& sceneCamera) :
		m_CameraTransform(camTransform),
		m_SceneCamera(sceneCamera)
	{
	}
	//TODO look at your other camera implemantations and rework this to function like those
	//TODO add controls for rotating Camera
	//TODO steal some code from my RTS camera made in Unity to improve controlls
	//TODO change so we move relative to our current rotation instead of set axies.
	//TODO make sure that all controllsare in relation to rotation and posiiton
	void CameraController::OnUpdate(TimeStep deltaTime)
	{

		UpdateInputs();

		float dt = deltaTime;

		//TODO normalize direction, lock mouse to view

		Input::SetDisplayCursor(!m_RightMousePressed);
		if (m_RightMousePressed)
		{
			
			float xDir = m_ViewCenter.x - Input::GetMouseX();
			float yDir = m_ViewCenter.y - Input::GetMouseY();

			m_CameraTransform.Yaw() += glm::radians(xDir) * m_Sensitivity * dt;
			m_CameraTransform.Pitch() += glm::radians(yDir) * m_Sensitivity * dt;

			Input::SetMousePosition(m_ViewCenter.x, m_ViewCenter.y);
			
		}
		

		


		if (m_DirInputs.any())
		{

			
				//Input Movment
				auto isCameraPesrpective = m_SceneCamera.Camera.GetIsPerspective();
				auto forward = isCameraPesrpective ? m_CameraTransform.GetForward() : glm::vec3(0.f, .0f, .0f);
				auto right = m_CameraTransform.GetRight();

				m_InputAxis.x = m_DirInputs[MoveDir::mBack] - m_DirInputs[MoveDir::mForward];
				m_InputAxis.y = m_DirInputs[MoveDir::mLeft] - m_DirInputs[MoveDir::mRight];
				m_InputAxis.z = m_DirInputs[MoveDir::mDown] - m_DirInputs[MoveDir::mUp];

				glm::vec3 dir = glm::normalize((forward * m_InputAxis.x) + (right * m_InputAxis.y) + (WorldUp * m_InputAxis.z));

				dir = glm::isnan(dir).b ? glm::vec3(0.f) : dir;
				m_CameraTransform.Position = m_CameraTransform.Position + (dir * m_CameraSpeed * dt);

				if (!isCameraPesrpective)
					m_SceneCamera.Camera.ChangeOrthoSize(m_InputAxis.x * m_CameraSpeed * dt);
		}

	}

	void CameraController::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(SOL_BIND_EVENT_FN(CameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(SOL_BIND_EVENT_FN(CameraController::OnWindowResized));
	}

	//TODO instead of handeling scrolling as events, 
	// do it in OnUpdate so we can get smoother and shorter zoom steps
	bool CameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{

		/*	m_ZoomLevel = glm::max(m_ZoomLevel- e.GetYOffset(),0.1f);
			m_Camera->SetProjection(m_AspectRatio * m_ZoomLevel);
			*/
		return false;
	}

	bool CameraController::OnWindowResized(WindowResizeEvent& e)
	{
		return false;
	}

	void CameraController::UpdateInputs()
	{
		m_DirInputs[MoveDir::mForward] = Input::IsKeyPressed(Key::W);
		m_DirInputs[MoveDir::mBack] = Input::IsKeyPressed(Key::S);
		m_DirInputs[MoveDir::mRight] = Input::IsKeyPressed(Key::D);
		m_DirInputs[MoveDir::mLeft] = Input::IsKeyPressed(Key::A);
		m_DirInputs[MoveDir::mUp] = Input::IsKeyPressed(Key::SPACE);
		m_DirInputs[MoveDir::mDown] = Input::IsKeyPressed(Key::LEFT_SHIFT);

		if (Input::IsKeyPressed(Key::P) && m_OldPerspectiveInput == false)
		{
			m_OldPerspectiveInput = true;
			m_PerspectiveToggle = !m_PerspectiveToggle;
			m_SceneCamera.Camera.SetIsPerspective(m_PerspectiveToggle);
		}
		else if (!Input::IsKeyPressed(Key::P))
		{
			m_OldPerspectiveInput = false;
		}

		if (Input::IsKeyPressed(Key::H))
		{
			m_CameraTransform.Position = glm::vec3(0.0f);
			m_CameraTransform.Rotation = glm::vec3(0.0f);
		}


		m_AltPressed = Input::IsKeyPressed(Key::LEFT_ALT);

		m_RightMousePressed = Input::IsMouseButtonPressed(1);
		if (m_OldRightMousePressed != m_RightMousePressed && m_RightMousePressed==true) 
		{
			Input::SetMousePosition(m_ViewCenter.x, m_ViewCenter.y);
		}
		m_OldRightMousePressed = m_RightMousePressed;

	}

}