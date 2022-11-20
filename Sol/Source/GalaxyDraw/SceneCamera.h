#pragma once

#include "Camera.h"

namespace GalaxyDraw 
{

	class SceneCamera : public Camera
	{
	public:
		SceneCamera();
		virtual ~SceneCamera() = default;
		void SetOrtho(float size, float nearClip, float farClip);

		void SetViewportSize(uint32_t width, uint32_t height);


		float GetOrthoSize() { return m_OrthoSize; }
		void SetOrthoSize(float size) { m_OrthoSize = size; RecalcProjection(); }

	private:

		void RecalcProjection();

		float m_OrthoSize = 10.f;
		float m_OrthoNear = -1.f;
		float m_OrthoFar = 1.f;

		float m_AspectRatio = 0;
	};
}