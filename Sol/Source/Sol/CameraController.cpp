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
		if (Input::IsKeyPressed(SOL_KEY_D))
		{
			
			m_CameraTransform.Position.x += m_CameraSpeed * deltaTime;
		}
		else if (Input::IsKeyPressed(SOL_KEY_A))
		{
			m_CameraTransform.Position.x += -m_CameraSpeed * deltaTime;
		}

		if (Input::IsKeyPressed(SOL_KEY_W))
		{
			m_CameraTransform.Position.y += m_CameraSpeed * deltaTime;
		}
		else if (Input::IsKeyPressed(SOL_KEY_S))
		{
			m_CameraTransform.Position.y += -m_CameraSpeed * deltaTime;
		}

		if (Input::IsKeyPressed(SOL_KEY_R))
		{
			
			m_CameraTransform.Rotation.x += m_CameraSpeed * deltaTime;
		}
		else if (Input::IsKeyPressed(SOL_KEY_F))
		{
			m_CameraTransform.Rotation.x += -m_CameraSpeed * deltaTime;
		}

		if (Input::IsKeyPressed(SOL_KEY_Q))
		{
			m_CameraTransform.Rotation.y += m_CameraSpeed * deltaTime;
		}
		else if (Input::IsKeyPressed(SOL_KEY_E))
		{
			m_CameraTransform.Rotation.y += -m_CameraSpeed  * deltaTime;
		}

		if (Input::IsKeyPressed(SOL_KEY_T))
		{
			m_CameraTransform.Rotation.z += m_CameraSpeed  * deltaTime;
		}
		else if (Input::IsKeyPressed(SOL_KEY_G))
		{
			m_CameraTransform.Rotation.z += -m_CameraSpeed * deltaTime;
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

}