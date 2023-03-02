#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>

namespace Sol
{
	struct TransformComp
	{
	public:

		TransformComp() = default;
		TransformComp(const TransformComp&) = default;
		TransformComp(const glm::vec3 position) : Position(position) { }
		TransformComp(const glm::vec3 position, const glm::vec3 rotation) : Position(position), Rotation(rotation) { }
		TransformComp(const glm::vec3 position, const glm::vec3 rotation, const glm::vec3 scale) : Position(position), Rotation(rotation), Scale(scale) { }
		~TransformComp() = default;

		glm::mat4 GetTransformMatrix() const
		{
			glm::mat4 transformMatrix = glm::translate(glm::mat4(1.0f), Position);
			glm::mat4 rotationMatrix = GetRotationMatrix();
			glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), Scale);

			glm::mat4 retval = transformMatrix * rotationMatrix * scaleMatrix;
			return SanitizeMatrix(transformMatrix * rotationMatrix * scaleMatrix);
		}

		glm::mat4 GetViewMatrix() const
		{
			glm::mat4 viewMatrix = glm::mat4(1.0f);

			viewMatrix = glm::translate(viewMatrix, -Position);
			viewMatrix = viewMatrix * GetRotationMatrix();
			viewMatrix = glm::inverse(viewMatrix);

			return SanitizeMatrix(viewMatrix);
		}

		glm::vec3 GetForward()
		{
			glm::mat4 rotationMatrix = GetRotationMatrix();

			glm::vec3 forward = glm::vec3(0.0f, 0.0f, -1.0f);
			forward = glm::normalize(glm::vec3(rotationMatrix * glm::vec4(forward, 0.0f)));

			return forward;
		}

		glm::vec3 GetUp()
		{
			glm::mat4 rotationMatrix = GetRotationMatrix();

			glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
			up = glm::normalize(glm::vec3(rotationMatrix * glm::vec4(up, 0.0f)));

			return up;
		}

		glm::vec3 GetRight()
		{
			glm::mat4 rotationMatrix = GetRotationMatrix();

			glm::vec3 right = glm::vec3(1.0f, 0.0f, 0.0f);
			right = glm::normalize(glm::vec3(rotationMatrix * glm::vec4(right, 0.0f)));

			return right;
		}


		operator const glm::mat4() const { return GetTransformMatrix(); }

		glm::mat4 GetRotationMatrix() const { return SanitizeMatrix(glm::toMat4(glm::quat(glm::radians(Rotation)))); }

		float& Pitch() { return Rotation.x = std::fmod(Rotation.x, 360.0f); }

		float& Yaw() { return Rotation.y = std::fmod(Rotation.y, 360.0f); }

		float& Roll() { return Rotation.z = std::fmod(Rotation.z, 360.0f); }

		glm::vec3 Position = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };

	private:
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