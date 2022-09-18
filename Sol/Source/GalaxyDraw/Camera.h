#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H


#include <glm/glm.hpp>
struct GLFWwindow;

namespace GalaxyDraw {
	class Shader;

	//TODO turn this into an interface and make OpenGL_Camera the implementation
	class Camera
	{
	public:
		Camera(int width, int height, glm::vec2 aspectRatio, glm::vec3 position);
		void SetProjection( glm::vec2 aspectRatio);

		const glm::vec3& GetPosition() { return m_Position; }
		void SetPosition(const glm::vec3& newPosition);
		

		const glm::vec3& GetRotation() { return m_Rotation; }
		void SetRotation(const glm::vec3& newRotation);
		

		const glm::mat4& GetViewMatrix() { return m_ViewMatrix; }
		const glm::mat4& GetProjectionMatrix() { return m_ProjectionMatrix; }
		const glm::mat4& GetCameraMatrix() { return m_CameraMatrix; }

		// Updates the camera matrix to the Vertex Shader
		void updateMatrix(float FOVdeg, float nearPlane, float farPlane);

		void MatrixUniform(Shader& shader, const char* uniform);

		void Inputs(GLFWwindow* window);

	private:

		void RecalculateViewMatrix();

		glm::vec3 m_Position = glm::vec3(0.0f);
		glm::vec3 m_Rotation = glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 m_UpDir = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::mat4 m_ViewMatrix = glm::mat4(1.0f);
		glm::mat4 m_ProjectionMatrix = glm::mat4(1.0f);
		glm::mat4 m_CameraMatrix = glm::mat4(1.0f);

		bool firstClick = true;

		int width;
		int height;

		float speed = 0.1f;
		float sensitivity = 110.0f;
	};
}
#endif