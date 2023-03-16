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

	void CameraController::OnUpdate(TimeStep deltaTime)
	{

		UpdateInputs();

		float dt = deltaTime;

		MouseNavigation(dt);

		ButtonNavigation(dt);

		ApplyCameraMovement(dt);

		if (m_AltMovePressed)
		{
			m_InputAxis.x = 0.f;
			m_InputAxis.y = 0.f;
			m_InputAxis.z = 0.f;
			m_MoveVelocity = 0.f;
		}


	}

	void CameraController::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(SOL_BIND_EVENT_FN(CameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(SOL_BIND_EVENT_FN(CameraController::OnWindowResized));
	}

	//TODO make scrolling move faster
	bool CameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		m_InputAxis.x = -e.GetYOffset();
		return false;
	}

	bool CameraController::OnWindowResized(WindowResizeEvent& e)
	{
		return false;
	}

	//TODO make keys bindable
	void CameraController::UpdateInputs()
	{
		m_DirInputs[MoveDir::mForward] = Input::IsKeyPressed(Key::W);
		m_DirInputs[MoveDir::mBack] = Input::IsKeyPressed(Key::S);
		m_DirInputs[MoveDir::mRight] = Input::IsKeyPressed(Key::D);
		m_DirInputs[MoveDir::mLeft] = Input::IsKeyPressed(Key::A);
		m_DirInputs[MoveDir::mUp] = Input::IsKeyPressed(Key::SPACE);
		m_DirInputs[MoveDir::mDown] = Input::IsKeyPressed(Key::F);

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

		
		m_AltMovePressed = Input::IsKeyPressed(Key::LEFT_ALT);

		//TODO make this a bindable mouse button
		m_MouseNavigationPressed = Input::IsMouseButtonPressed(1);

		if (m_OldMouseNavigationPressed != m_MouseNavigationPressed && m_MouseNavigationPressed == true)
		{
			Input::SetMousePosition(m_ViewCenter.x, m_ViewCenter.y);
		}
		m_OldMouseNavigationPressed = m_MouseNavigationPressed;

	}

	void CameraController::MouseNavigation(float dt)
	{
		Input::SetDisplayCursor(!m_MouseNavigationPressed);
		if (m_MouseNavigationPressed)
		{
			float xDir = m_ViewCenter.x - Input::GetMouseX();
			float yDir = m_ViewCenter.y - Input::GetMouseY();

			if (m_AltMovePressed)
			{
				//Mouse Panning
				m_InputAxis.y = -xDir;
				m_InputAxis.z = yDir;
			}
			else
			{
				//Mouse Rotation
				m_CameraTransform.Yaw() += glm::radians(xDir) * m_Sensitivity * dt;
				m_CameraTransform.Pitch() += glm::radians(yDir) * m_Sensitivity * dt;
			}
			Input::SetMousePosition(m_ViewCenter.x, m_ViewCenter.y);
		}
	}


	void CameraController::ButtonNavigation(float dt)
	{
		if (m_DirInputs.any() && !m_AltMovePressed)
		{
			//Button Movement
			m_InputAxis.x = m_DirInputs[MoveDir::mBack] - m_DirInputs[MoveDir::mForward];
			m_InputAxis.y = m_DirInputs[MoveDir::mLeft] - m_DirInputs[MoveDir::mRight];
			m_InputAxis.z = m_DirInputs[MoveDir::mDown] - m_DirInputs[MoveDir::mUp];

			m_MoveVelocity = glm::clamp(m_MoveVelocity + (m_MoveAccelerationSpeed * dt), 0.f, 1.f);
		}
		else
		{
			m_MoveVelocity = glm::clamp(m_MoveVelocity - (m_MoveDecelerationSpeed * dt), 0.f, 1.f);
		}
	}

	void CameraController::ApplyCameraMovement(float dt)
	{

		auto isCameraPesrpective = m_SceneCamera.Camera.GetIsPerspective();
		auto forward = isCameraPesrpective ? m_CameraTransform.GetForward() : glm::vec3(0.f, .0f, .0f);
		auto right = m_CameraTransform.GetRight();

		glm::vec3 dir = glm::normalize((forward * m_InputAxis.x) + (right * m_InputAxis.y) + (WorldUp * m_InputAxis.z));
		dir = dir * (!m_AltMovePressed ? m_MoveVelocity : 1.f);

		dir = glm::isnan(dir).b ? glm::vec3(0.f) : dir;
		m_CameraTransform.Position = m_CameraTransform.Position + (dir * m_CameraSpeed * dt);

		if (!isCameraPesrpective)
			m_SceneCamera.Camera.ChangeOrthoSize(m_InputAxis.x * m_CameraSpeed * dt);
	}

}