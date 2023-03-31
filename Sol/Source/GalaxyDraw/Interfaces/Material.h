#pragma once
#include <glm/glm.hpp>

namespace GalaxyDraw {

	struct Material
	{
		uint32_t TextureIndex;//TextureIndices[5];
		glm::vec4 Color;
		// Other material properties go here
	};
}