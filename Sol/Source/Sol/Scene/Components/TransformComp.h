#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

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
			glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));

			return glm::translate(glm::mat4(1.0f), Position)
				* rotation
				* glm::scale(glm::mat4(1.0f), Scale);
		}

		glm::vec3 GetForward()
		{
			glm::vec3 dir;
			float pitch = glm::radians(Pitch());
			float yaw = glm::radians(Yaw());

			dir.x = sin(yaw) * cos(pitch);
			dir.y = -sin(pitch);
			dir.z = -cos(yaw) * cos(pitch);

			return glm::normalize(dir);
		}

		glm::vec3 GetUp()
		{
			glm::vec3 up;
			float pitch = glm::radians(Pitch());
			float yaw = glm::radians(Yaw());
			float roll = glm::radians(Roll());

			up.x = sin(pitch) * sin(yaw) * cos(roll) + cos(pitch) * sin(roll);
			up.y = cos(pitch) * cos(roll);
			up.z = -sin(yaw) * cos(pitch) * cos(roll) + sin(pitch) * sin(roll);

			return glm::normalize(up);
		}

		glm::vec3 GetRight()
		{
			glm::vec3 right;
			float pitch = glm::radians(Pitch());
			float yaw = glm::radians(Yaw());
			float roll = glm::radians(Roll());

			right.x = cos(yaw) * cos(roll) - sin(yaw) * sin(pitch) * sin(roll);
			right.y = sin(yaw) * cos(pitch);
			right.z = cos(yaw) * sin(roll) + sin(yaw) * sin(pitch) * cos(roll);

			return glm::normalize(right);
		}

		operator const glm::mat4() const {
			return GetTransformMatrix();
		}

		float& Pitch() {
			return Rotation.x;
		}

		float& Yaw() {
			return Rotation.y;
		}

		float& Roll() {
			return Rotation.z;
		}

		glm::vec3 Position = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };

	};
}