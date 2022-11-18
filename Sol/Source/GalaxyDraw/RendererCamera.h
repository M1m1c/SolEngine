#pragma once
#include <glm/glm.hpp>

namespace GalaxyDraw {
	class RendererCamera
	{
	public:
		RendererCamera() = default;
		RendererCamera(glm::mat4 projection) :m_Projection(projection) {};
		virtual ~RendererCamera() = default;

		const glm::mat4& GetProjection() { return m_Projection; }

	protected:
		glm::mat4 m_Projection = glm::mat4(1.f);
	};

}