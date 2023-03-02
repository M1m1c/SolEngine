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

			return transformMatrix * rotationMatrix * scaleMatrix;
		}

		glm::mat4 GetViewMatrix() const
		{
			glm::mat4 viewMatrix = glm::mat4(1.0f);

			viewMatrix = glm::translate(viewMatrix, -Position);
			viewMatrix = viewMatrix * glm::toMat4(glm::quat(glm::vec3(Rotation)));
			viewMatrix = glm::inverse(viewMatrix);

			return viewMatrix;
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

		glm::mat4 GetRotationMatrix() const { return glm::toMat4(glm::quat(Rotation)); }

		float& Pitch() { return Rotation.x; }

		float& Yaw() { return Rotation.y; }

		float& Roll() { return Rotation.z; }

		glm::vec3 Position = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };

	};
}