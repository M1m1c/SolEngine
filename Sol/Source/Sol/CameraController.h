#pragma once
#include "GalaxyDraw/GalaxyDraw.h"
#include <Sol/Core/TimeStep.h>

#include "Sol/Events/ApplicationEvent.h"
#include "Sol/Events/KeyEvent.h"
#include "Sol/Events/MouseEvent.h"

namespace Sol 
{
	class CameraController
	{
	public:
		CameraController(int width, int height, glm::vec2 aspectRatio, glm::vec3 position);

		void OnUpdate(TimeStep deltaTime);
		void OnEvent(Event& e);

		GD_Camera& GetCamera() { return m_Camera; }
		const GD_Camera& GetCamera() const { return m_Camera; }

	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);


		glm::vec2 m_AspectRatio;
		float m_ZoomLevel = 1.0f;
		GD_Camera m_Camera;
		//bool m_EnableRotation;

		glm::vec3 m_CameraPosition = glm::vec3(0.f);
		float m_CameraSpeed = 1.f;

	};


}