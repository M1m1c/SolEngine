#pragma once
#include <glm/glm.hpp>

namespace GalaxyDraw {

	struct AmbientLight
	{
		glm::vec3 AmbientColor;
		float Intensity;

		AmbientLight() {
			Intensity = 0.f;
			AmbientColor = glm::vec3(1.f, 1.f, 1.f);
		}

		AmbientLight(float intensity, glm::vec3 ambientColor)
			: Intensity(intensity), AmbientColor(ambientColor)
		{}
	};

	struct DirectionalLight
	{
		AmbientLight Ambient;
		float DiffuseIntensity=1.f;
	};
}