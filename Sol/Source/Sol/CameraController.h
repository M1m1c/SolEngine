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
		CameraController(TransformComp& camTransform, CameraComp& sceneCamera);

		void OnUpdate(TimeStep deltaTime);
		void OnEvent(Event& e);

	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);
	
		TransformComp& m_CameraTransform;
		CameraComp& m_SceneCamera;
		
		float m_CameraSpeed = 1.f;

	};


}