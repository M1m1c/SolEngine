#pragma once
#include "GalaxyDraw/GalaxyDraw.h"
#include <Sol/Core/TimeStep.h>

#include "Sol/Events/ApplicationEvent.h"
#include "Sol/Events/KeyEvent.h"
#include "Sol/Events/MouseEvent.h"
#include <bitset>

namespace Sol
{
	static const  glm::vec3 WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	
	class CameraController
	{
	public:
		CameraController(TransformComp& camTransform, CameraComp& sceneCamera);

		void OnUpdate(TimeStep deltaTime);
		void OnEvent(Event& e);

	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);

		void UpdateInputs();

		TransformComp& m_CameraTransform;
		CameraComp& m_SceneCamera;

		glm::vec3 m_InputAxis{ 0.f,0.f,0.f };
		std::bitset<6> m_DirInputs;
		std::bitset<4> m_RotInputs;

		float m_CameraSpeed = 3.f;
		float m_RotSpeed = 60.f;
		float m_Sensitivity = 1.f;

		enum MoveDir
		{
			mForward = 0,
			mBack = 1,
			mRight = 2,
			mLeft = 3,
			mUp = 4,
			mDown = 5
		};

		enum RotDir
		{
			rRight = 0,
			rLeft = 1,
			rUp = 2,
			rDown = 3
		};
	};


}