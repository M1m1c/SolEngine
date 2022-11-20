#pragma once

#include <glm/glm.hpp>

namespace GalaxyDraw 
{
	class OrthoCamera
	{
	public:
		static std::shared_ptr<OrthoCamera> Create(int width, int height, glm::vec2 aspectRatio, glm::vec3 position);


		virtual void SetProjection(glm::vec2 aspectRatio) = 0;

		virtual const glm::vec3& GetPosition() = 0;
		virtual void SetPosition(const glm::vec3& newPosition) = 0;


		virtual const glm::vec3& GetRotation() = 0;
		virtual void SetRotation(const glm::vec3& newRotation) = 0;


		virtual const glm::mat4& GetViewMatrix() = 0;
		virtual const glm::mat4& GetProjectionMatrix() = 0;
		virtual const glm::mat4& GetCameraMatrix() = 0;

	};
}