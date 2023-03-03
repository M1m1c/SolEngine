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
		void SetPerspective(float FOV, float nearClip, float farClip);

		void SetViewportSize(uint32_t width, uint32_t height);


		float GetOrthoSize() { return m_OrthoSize; }
		void SetOrthoSize(float size) { m_OrthoSize = size; RecalcProjection(); }
		void ChangeOrthoSize(float sizeChange);

		void SetFOV(float FOV) { m_FOV = FOV; RecalcProjection(); }
		float GetFOV() { return m_FOV; }

		bool GetIsPerspective() { return m_IsPerspective; }
		void SetIsPerspective(bool b) { m_IsPerspective = b; RecalcProjection();}

	private:

		void RecalcProjection();

		float m_AspectRatio = 1.0f;

		float m_OrthoSize = 10.f;
		float m_ONearClippingPlane = -10.f;
		float m_OFarClippingPlane = 100.f;

		float m_FOV = 60.f;
		float m_PNearClippingPlane = 0.1f;
		float m_PFarClippingPlane = 100.f;

		bool m_IsPerspective = true;

		glm::mat4 SanitizeMatrix(const glm::mat4& matrix) const
		{
			glm::mat4 sanitizedMatrix = matrix;

			// Loop through each element in the matrix
			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					// Check if the element is -0.0
					if (sanitizedMatrix[i][j] == -0.0f)
					{
						sanitizedMatrix[i][j] = 0.0f;
					}
				}
			}

			return sanitizedMatrix;
		}
	};
}