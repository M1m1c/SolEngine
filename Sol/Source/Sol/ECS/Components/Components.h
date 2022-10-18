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
		glm::mat4 m_Transform{ 1.f };
	};

	class SpriteRendererComp : public IComponent
	{
	public:

		SpriteRendererComp() = default;
		SpriteRendererComp(const SpriteRendererComp&) = default;
		SpriteRendererComp(const glm::vec4& color) : m_Color(color) { }
		~SpriteRendererComp() = default;

		operator glm::vec4& () { return m_Color; }
		operator const glm::vec4& () const { return m_Color; }

	private:
		glm::vec4 m_Color{ 1.f };
	};
}