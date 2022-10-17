#pragma once

#include <glm/glm.hpp>
#include "Sol/ECS/Interfaces/IComponent.h"

namespace Sol
{

	class TransformComp : public IComponent
	{
	public:

		TransformComp() = default;
		TransformComp(const TransformComp&) = default;
		TransformComp(const glm::mat4& transform) : m_Transform(transform) { }
		~TransformComp() = default;

		operator glm::mat4& () { return m_Transform; }
		operator const glm::mat4& () const { return m_Transform; }

	private:
		glm::mat4 m_Transform = glm::mat4(1.f);
	};
}