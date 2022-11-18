#pragma once
#include <glm/glm.hpp>

namespace Sol {
	class RuntimeCamera
	{
	public:
		RuntimeCamera(glm::mat4 projection) :m_Projection(projection) {};

		const glm::mat4& GetProjection() { return m_Projection; }

	private:
		glm::mat4 m_Projection;
	};

}