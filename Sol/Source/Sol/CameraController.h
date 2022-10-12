#pragma once
#include "GalaxyDraw/GalaxyDraw.h"
#include <Sol/Core/TimeStep.h>

#include "Sol/Events/ApplicationEvent.h"
#include "Sol/Events/KeyEvent.h"
#include "Sol/Events/MouseEvent.h"

namespace Sol 
{
	struct OrthoCameraBounds
	{
		float Left, Right;
		float Bottom, Top;

		float GetWidth() { return Right - Left; }
		float GetHeight() { return Top - Bottom; }
	};


	class CameraController
	{
	public:
		CameraController(int width, int height, glm::vec2 aspectRatio, glm::vec3 position);

		void OnUpdate(TimeStep deltaTime);
		void OnEvent(Event& e);

		void OnResize(uint32_t width, uint32_t height);

		GD_Camera& GetCamera() { return *m_Camera; }
		const GD_Camera& GetCamera() const { return *m_Camera; }

	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);


		glm::vec2 m_AspectRatio;
		float m_ZoomLevel = 1.0f;
		s_ptr<GD_Camera> m_Camera;
		//OrthoCameraBounds m_Bounds;
		//bool m_EnableRotation;

		glm::vec3 m_CameraPosition = glm::vec3(0.f);
		glm::vec3 m_CameraRotation= glm::vec3(0.f);
		float m_CameraSpeed = 1.f;

	};


}