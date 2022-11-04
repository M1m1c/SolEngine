#pragma once

#include <glm/glm.hpp>

namespace Sol
{
	struct NameComp
	{
		NameComp() = default;
		NameComp(const NameComp&) = default;
		NameComp(const std::string& name) :m_Name(name) {}
		~NameComp() = default;

		operator std::string& () { return m_Name; }
		operator const std::string& () const { return m_Name; }
	private:
		std::string m_Name;
	};

	struct TransformComp
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

	struct SpriteRendererComp
	{
	public:

		SpriteRendererComp() = default;
		SpriteRendererComp(const SpriteRendererComp&) = default;
		SpriteRendererComp(const glm::vec4& color) : Color(color) { }
		~SpriteRendererComp() = default;

		/*operator glm::vec4& () { return m_Color; }
		operator const glm::vec4& () const { return m_Color; }*/

		glm::vec4 Color{ 1.f };

	private:
		
	};
}