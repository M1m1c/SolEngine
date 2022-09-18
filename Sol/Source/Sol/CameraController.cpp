#include "solpch.h"
#include "CameraController.h"
#include "Sol/Input.h"
#include <Sol/KeyCodes.h>

namespace Sol
{
	CameraController::CameraController(int width, int height, glm::vec2 aspectRatio, glm::vec3 position) : 
		m_AspectRatio(aspectRatio),
		m_Camera(width, height, aspectRatio* m_ZoomLevel, position)
	{
	}

	//TODO add controls for rotating Camera
	//TODO steal some code from my RTS camera made in Unity to improve controlls
	//TODO change so we move relative to our current rotation instead of set axies.
	void CameraController::OnUpdate(TimeStep deltaTime)
	{
		if (Input::IsKeyPressed(SOL_KEY_D))
		{
			m_CameraPosition.x += m_CameraSpeed * deltaTime;
		}
		else if (Input::IsKeyPressed(SOL_KEY_A))
		{
			m_CameraPosition.x += -m_CameraSpeed * deltaTime;
		}

		if (Input::IsKeyPressed(SOL_KEY_W))
		{
			m_CameraPosition.y += m_CameraSpeed * deltaTime;
		}
		else if (Input::IsKeyPressed(SOL_KEY_S))
		{
			m_CameraPosition.y += -m_CameraSpeed * deltaTime;
		}

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation({ 0.5f,0.0f,0.f });
		/*if (m_EnableRotation) 
		{

		}*/

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
		
		m_ZoomLevel = glm::max(m_ZoomLevel- e.GetYOffset(),0.1f);
		m_Camera.SetProjection(m_AspectRatio * m_ZoomLevel);
		return false;
	}

	bool CameraController::OnWindowResized(WindowResizeEvent& e)
	{
		auto tempRatio = (float)e.GetWidth() / (float)e.GetHeight();
		m_AspectRatio = glm::vec2(tempRatio * m_ZoomLevel, m_ZoomLevel);
		m_Camera.SetProjection(m_AspectRatio * m_ZoomLevel);
		return false;
	}

}