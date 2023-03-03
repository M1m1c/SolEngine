#include "solpch.h"
#include "SceneCamera.h"
#include <glm/gtc/matrix_transform.hpp>

namespace GalaxyDraw
{
	SceneCamera::SceneCamera()
	{
		RecalcProjection();
	}

	void SceneCamera::ChangeOrthoSize(float sizeChange)
	{
		SetOrtho(glm::max(m_OrthoSize + sizeChange, 0.1f), m_NearClippingPlane, m_FarClippingPlane);
	}

	void SceneCamera::SetOrtho(float size, float nearClip, float farClip)
	{
		m_OrthoSize = size;
		m_NearClippingPlane = nearClip;
		m_FarClippingPlane = farClip;
		RecalcProjection();
	}

	void SceneCamera::SetViewportSize(uint32_t width, uint32_t height)
	{
		m_AspectRatio = (float)width / (float)height;
		RecalcProjection();
	}

	void SceneCamera::RecalcProjection()
	{
		if (m_IsPerspective)
		{
			m_Projection = glm::perspective(glm::radians(m_FOV), m_AspectRatio, m_NearClippingPlane, m_FarClippingPlane);
		}
		else
		{
			float orhtoLeft = -m_OrthoSize * m_AspectRatio * 0.5f;
			float orhtoRight = m_OrthoSize * m_AspectRatio * 0.5f;
			float orhtoBottom = -m_OrthoSize * 0.5f;
			float orhtoTop = m_OrthoSize * 0.5f;

			m_Projection = SanitizeMatrix(
				glm::ortho(
					orhtoLeft, orhtoRight,
					orhtoBottom, orhtoTop,
					m_NearClippingPlane, m_FarClippingPlane)
			);
		}
	}
}
