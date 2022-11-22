#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H

#include "GalaxyDraw/Interfaces/OrthoCamera.h"

struct GLFWwindow;

namespace GalaxyDraw {
	class Shader;

	//TODO turn this into an interface and make OpenGL_Camera the implementation
	class OpenGL_OrthoCamera : public OrthoCamera
	{
	public:
		OpenGL_OrthoCamera(int width, int height, glm::vec2 aspectRatio, glm::vec3 position);
		virtual void SetProjection( glm::vec2 aspectRatio) override;

		virtual const glm::vec3& GetPosition() override { return m_Position; }
		virtual void SetPosition(const glm::vec3& newPosition) override;
		

		virtual const glm::vec3& GetRotation() override { return m_Rotation; }
		virtual void SetRotation(const glm::vec3& newRotation)override;
		

		virtual const glm::mat4& GetViewMatrix() const override { return m_ViewMatrix; }
		virtual const glm::mat4& GetProjectionMatrix() const override { return m_ProjectionMatrix; }
		virtual const glm::mat4& GetViewProjectionMatrix() const override { return m_ViewProjectionMatrix; }

		// Updates the camera matrix to the Vertex Shader
		void updateMatrix(float FOVdeg, float nearPlane, float farPlane);

		void MatrixUniform(Shader& shader, const char* uniform);

	private:

		void RecalculateViewMatrix();

		glm::vec3 m_Position = glm::vec3(0.0f);
		glm::vec3 m_Rotation = glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 m_UpDir = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::mat4 m_ViewMatrix = glm::mat4(1.0f);
		glm::mat4 m_ProjectionMatrix = glm::mat4(1.0f);
		glm::mat4 m_ViewProjectionMatrix = glm::mat4(1.0f);

		const  glm::vec3 worldForward = glm::vec3(0.0f, 0.0f, -1.0f);
		const  glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
		const  glm::vec3 worldRight = glm::vec3(1.0f, 0.0f, 0.0f);

		bool firstClick = true;

		int width;
		int height;

		float speed = 0.1f;
		float sensitivity = 110.0f;
	};
}
#endif