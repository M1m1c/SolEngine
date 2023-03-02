#include "solpch.h"
#include "CameraController.h"
#include "Sol/Core/Input.h"
#include "Sol/Core/KeyCodes.h"

namespace Sol
{

	CameraController::CameraController(TransformComp& camTransform, CameraComp& sceneCamera):
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

		if (m_DirInputs.any())
		{
			auto forward = m_CameraTransform.GetForward();
			auto right = m_CameraTransform.GetRight();//glm::normalize(glm::cross(forward, WorldUp));

			m_InputAxis.x = (m_DirInputs[MoveDir::mForward] == 1 ? -1.f : (m_DirInputs[MoveDir::mBack] == 1 ? 1.f : 0.f));
			m_InputAxis.y = (m_DirInputs[MoveDir::mRight] == 1 ? 1.f : (m_DirInputs[MoveDir::mLeft] == 1 ? -1.f : 0.f));
			m_InputAxis.z = (m_DirInputs[MoveDir::mUp] == 1 ? 1.f : (m_DirInputs[MoveDir::mDown] == 1 ? -1.f : 0.f));

			glm::vec3 dir = glm::normalize((forward * m_InputAxis.x) + (right * m_InputAxis.y) + (WorldUp * m_InputAxis.z));

			dir = glm::isnan(dir).b ? glm::vec3(0.f) : dir;
			m_CameraTransform.Position = m_CameraTransform.Position + (dir * m_CameraSpeed * dt);
			m_SceneCamera.Camera.ChangeOrthoSize(m_InputAxis.x * dt);
		}

		//Rotation
		float camYawDir = 0;
		float camPitchDir = 0;

		if (m_RotInputs.any())
		{
			camYawDir = (m_RotInputs[RotDir::rRight] == 1 ? -m_RotSpeed : (m_RotInputs[RotDir::rLeft] == 1 ? m_RotSpeed : 0.f));
			camPitchDir = (m_RotInputs[RotDir::rUp] == 1 ? m_RotSpeed : (m_RotInputs[RotDir::rDown] == 1 ? -m_RotSpeed : 0.f));
		}
		m_CameraTransform.Yaw() += glm::radians(camYawDir) * m_Sensitivity * deltaTime;
		m_CameraTransform.Pitch() += glm::radians(camPitchDir) * m_Sensitivity * deltaTime;


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
		m_DirInputs[MoveDir::mForward] = Input::IsKeyPressed(SOL_KEY_W);
		m_DirInputs[MoveDir::mBack] = Input::IsKeyPressed(SOL_KEY_S);
		m_DirInputs[MoveDir::mRight] = Input::IsKeyPressed(SOL_KEY_D);
		m_DirInputs[MoveDir::mLeft] = Input::IsKeyPressed(SOL_KEY_A);
		m_DirInputs[MoveDir::mUp] = Input::IsKeyPressed(SOL_KEY_SPACE);
		m_DirInputs[MoveDir::mDown] = Input::IsKeyPressed(SOL_KEY_LEFT_SHIFT);

		m_RotInputs[RotDir::rRight] = Input::IsKeyPressed(SOL_KEY_E);
		m_RotInputs[RotDir::rLeft] = Input::IsKeyPressed(SOL_KEY_Q);
		m_RotInputs[RotDir::rUp] = Input::IsKeyPressed(SOL_KEY_R);
		m_RotInputs[RotDir::rDown] = Input::IsKeyPressed(SOL_KEY_F);
	}

}