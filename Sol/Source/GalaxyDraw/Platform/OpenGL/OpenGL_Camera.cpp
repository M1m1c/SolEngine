#include "solpch.h"
#include "OpenGL_Camera.h"

#include "GLMacros.h"
#include "GalaxyDraw/Interfaces/Shader.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

namespace GalaxyDraw {
	
	OpenGL_Camera::OpenGL_Camera(int width, int height,glm::vec2 aspectRatio, glm::vec3 position)
	{
		OpenGL_Camera::width = width;
		OpenGL_Camera::height = height;
		m_Position = position;
		//16:9 aspect ratio
		m_ProjectionMatrix = glm::ortho(-aspectRatio.x, aspectRatio.x, -aspectRatio.y, aspectRatio.y);
		//m_ProjectionMatrix = glm::ortho(-1.6f, 1.6f, -.9f, .9f);
		m_CameraMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void OpenGL_Camera::SetProjection(glm::vec2 aspectRatio)
	{
		m_ProjectionMatrix = glm::ortho(-aspectRatio.x, aspectRatio.x, -aspectRatio.y, aspectRatio.y);
		m_CameraMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void OpenGL_Camera::updateMatrix(float FOVdeg, float nearPlane, float farPlane)
	{
		// Makes camera look in the right direction from the right position
		m_ViewMatrix = glm::lookAt(m_Position, m_Position + m_Rotation, m_UpDir);
		// Adds perspective to the scene

		m_ProjectionMatrix = glm::perspective(glm::radians(FOVdeg), (float)width / height, nearPlane, farPlane);

		glm::mat4 ortho = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f);

		// Sets new camera matrix
		m_CameraMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void OpenGL_Camera::MatrixUniform(GalaxyDraw::Shader& shader, const char* uniform)
	{
		// Exports camera matrix
		GLCall(glUniformMatrix4fv(glGetUniformLocation(shader.GetID(), uniform), 1, GL_FALSE, glm::value_ptr(m_CameraMatrix)));
	}

	//TODO move all input to camera controller class
	void OpenGL_Camera::Inputs(GLFWwindow* window)
	{
		// Handles key inputs
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		{
			m_Position += speed * m_Rotation;
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		{
			m_Position += speed * -glm::normalize(glm::cross(m_Rotation, m_UpDir));
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		{
			m_Position += speed * -m_Rotation;
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		{
			m_Position += speed * glm::normalize(glm::cross(m_Rotation, m_UpDir));
		}
		if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		{
			m_Position += speed * m_UpDir;
		}
		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		{
			m_Position += speed * -m_UpDir;
		}
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		{
			speed = 0.2f;
		}
		else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
		{
			speed = 0.07f;
		}

		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

			if (firstClick)
			{
				glfwSetCursorPos(window, (width / 2), (height / 2));
				firstClick = false;
			}

			double mouseX;
			double mouseY;
			glfwGetCursorPos(window, &mouseX, &mouseY);

			float rotX = sensitivity * (float)(mouseY - (height / 2)) / height;
			float rotY = sensitivity * (float)(mouseX - (width / 2)) / width;

			glm::vec3 newOrientation = glm::rotate(m_Rotation, glm::radians(-rotX), glm::normalize(glm::cross(m_Rotation, m_UpDir)));

			if (abs(glm::angle(newOrientation, m_UpDir) - glm::radians(90.0f)) <= glm::radians(85.0f))
			{
				m_Rotation = newOrientation;
			}

			m_Rotation = glm::rotate(m_Rotation, glm::radians(-rotY), m_UpDir);

			glfwSetCursorPos(window, (width / 2), (height / 2));
		}
		else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE)
		{
			// Unhides cursor since camera is not looking around anymore
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			// Makes sure the next time the camera looks around it doesn't jump
			firstClick = true;
		}

	}

	void OpenGL_Camera::RecalculateViewMatrix()
	{
		m_ViewMatrix = m_ViewMatrix = glm::lookAt(m_Position, m_Position + m_Rotation, m_UpDir);
		m_CameraMatrix = m_ProjectionMatrix * m_ViewMatrix;
		/*glm::translate(glm::mat4(1.0f), m_Position) *
			glm::rotate(glm::mat4(1.0f), 1.0f, m_Rotation);*/
	}

	void OpenGL_Camera::SetPosition(const glm::vec3& newPosition)
	{
		m_Position = newPosition;
		RecalculateViewMatrix();
	}

	void OpenGL_Camera::SetRotation(const glm::vec3& newRotation)
	{
		//m_rotation is forward direction
		//m_UpDir is up direction
		//the cross of these is the right direction

		//Order of rotations is 
		//1. Z = roll of camera
		//2. X = yaw of camera
		//3. y = pitch of camera

		m_UpDir = glm::rotate(worldUp, glm::radians(-newRotation.z), worldForward);
		m_Rotation = glm::rotate(glm::vec3(0.0f, 0.0f, -1.0f), glm::radians(-newRotation.x), m_UpDir);
		auto right = glm::normalize(glm::cross(m_Rotation, m_UpDir));
		m_Rotation = glm::rotate(m_Rotation, glm::radians(-newRotation.y), right);
		RecalculateViewMatrix();
	}
}
