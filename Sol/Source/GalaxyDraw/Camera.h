#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H


#include <glm/glm.hpp>
struct GLFWwindow;

namespace GalaxyDraw {
	class Shader;

	class Camera
	{
	public:
		glm::vec3 Position;
		glm::vec3 Orientation = glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::mat4 cameraMatrix = glm::mat4(1.0f);

		bool firstClick = true;

		int width;
		int height;

		float speed = 0.1f;
		float sensitivity = 110.0f;

		Camera(int width, int height, glm::vec3 position);

		// Updates the camera matrix to the Vertex Shader
		void updateMatrix(float FOVdeg, float nearPlane, float farPlane);

		void MatrixUniform(Shader& shader, const char* uniform);

		void Inputs(GLFWwindow* window);
	};
}
#endif