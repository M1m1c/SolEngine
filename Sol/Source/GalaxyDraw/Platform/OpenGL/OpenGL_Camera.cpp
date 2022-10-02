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
