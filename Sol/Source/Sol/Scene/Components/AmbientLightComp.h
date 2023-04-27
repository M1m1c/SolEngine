#pragma once
#include <glm/glm.hpp>
namespace Sol
{
	struct AmbientLightComp
	{
		float Intensity;
		glm::vec3 AmbientColor;

		AmbientLightComp() {
			Intensity = 0.f;
			AmbientColor = glm::vec3(1.f, 1.f, 1.f);
		}

		AmbientLightComp(float intensity, glm::vec3 ambientColor) 
			: Intensity(intensity), AmbientColor(ambientColor)
		{}
	};
}