#pragma once
#include <glm/glm.hpp>
namespace Sol
{
	struct AmbientLightComp
	{
		float Intensity;
		glm::vec3 AmbientColor;

		AmbientLightComp(float intensity, glm::vec3 ambientColor) 
			: Intensity(intensity), AmbientColor(ambientColor)
		{}
	};
}