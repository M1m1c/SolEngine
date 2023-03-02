#pragma once

#include "Camera.h"

namespace GalaxyDraw 
{

	class SceneCamera : public Camera
	{
	public:
		SceneCamera();
		virtual ~SceneCamera() = default;
		void ChangeOrthoSize(float sizeChange);
		void SetOrtho(float size, float nearClip, float farClip);

		void SetViewportSize(uint32_t width, uint32_t height);


		float GetOrthoSize() { return m_OrthoSize; }
		void SetOrthoSize(float size) { m_OrthoSize = size; RecalcProjection(); }

		bool GetIsPerspective() { return m_IsPerspective; }
		void SetIsPerspective(bool b) { m_IsPerspective = b; }

	private:

		void RecalcProjection();

		float m_OrthoSize = 10.f;
		float m_OrthoNear = -10.f;
		float m_OrthoFar = 10.f;

		float m_AspectRatio = 1.0f;

		bool m_IsPerspective = false;
	};
}